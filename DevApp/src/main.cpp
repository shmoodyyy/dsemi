#include <dsemi/Dsemi.h>
#include "scenes/testscene2d.h"
#include <dsemi/graphics/color.h>
#include <dsemi/graphics/api_include.h>
#include <dsemi/graphics/color.h>

#include <dsemi/graphics/resources/resource_include.h>

// temporary includes for writing the graphics abstractions
#include <dsemi/graphics/render_target.h>

class DevApp : public dsemi::application
{
public:
	DevApp()
	{
	}

	~DevApp()
	{
	}

private:

	virtual void _on_init() override
	{
		active_scene = &_test_scene;
		_gfx_window.set_event_callback(BIND_EVENT(DevApp::on_event));
		_gfx_window.create(1280, 720, L"GFX Rendering Test Window");

		_device = dsemi::graphics::device::get();
		_dx_device = _device->get_dx_device();
		_dx_context = _device->get_context();
		_init_directx();
	}

	virtual void on_event(dsemi::ievent& e) override
	{
		dsemi::event_dispatcher dispatcher(e);
		dispatcher.dispatch<dsemi::window_close_event>(BIND_EVENT(DevApp::_on_window_close));
		dispatcher.dispatch<dsemi::window_resize_event>(BIND_EVENT(DevApp::_on_window_resize));
	}

	virtual void on_update(const float dt) override
	{
		//std::cout << "Frametime: " << dt * 1000 << "ms" << std::endl;
		_draw_triangle();
	}

	bool _on_window_close(dsemi::window_close_event &e)
	{
		// todo: add labels/tags to windows to be accessed
		_running = false;
		return true;
	}

	bool _on_window_resize(dsemi::window_resize_event& e)
	{
		if (_dx_context)
		{
			HRESULT hr;
			DXGI_SWAP_CHAIN_DESC desc;

			_rt_view = nullptr;

			_dx_swap_chain->GetDesc(&desc);
			_dx_swap_chain->ResizeBuffers(desc.BufferCount, 0u, 0u, DXGI_FORMAT_UNKNOWN, desc.Flags);

			ComPtr<ID3D11Texture2D> framebuf = nullptr;
			GFX_THROW_FAILED(_dx_swap_chain->GetBuffer(
				0u,
				__uuidof(ID3D11Texture2D),
				&framebuf
			));

			GFX_THROW_FAILED(_dx_device->CreateRenderTargetView(
				framebuf.Get(),
				0u,
				&_rt_view
			));
			_viewport.Width  = _gfx_window.width();
			_viewport.Height = _gfx_window.height();
			_dx_context->RSSetViewports(1u, &_viewport);

			// update constant buffer for view dimensions
			float view_w = _gfx_window.width();
			float view_h = _gfx_window.height();
			std::vector<float> view_scale_2d = {
				view_w,
				view_h
			};
			// send new view dimensions to GPU memory
			_dx_context->UpdateSubresource(_view_const_buffer.Get(), 0u, NULL, view_scale_2d.data(), sizeof(float) * view_scale_2d.size(), 0u);
			//_dx_context->VSSetConstantBuffers(0u, 1u, _view_const_buffer.GetAddressOf());
		}
		return true;
	}

private:
	test_scene2d _test_scene;
	dsemi::window _gfx_window;

private:

	void _init_directx()
	{
		_clear_color = 0.0f;
		HRESULT hr;

		// =======================================================
		//		CREATE SWAP CHAIN
		// =======================================================
		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferDesc.Width = _gfx_window.width();
		sd.BufferDesc.Height = _gfx_window.height();
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.SampleDesc.Count = 1u;
		sd.SampleDesc.Quality = 0u;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 2u;
		sd.OutputWindow = _gfx_window.get_hwnd();
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		auto dxgi_factory = _device->get_dx_factory();
		GFX_THROW_FAILED(dxgi_factory->CreateSwapChain(
			_dx_device.Get(),
			&sd,
			&_dx_swap_chain
		));

 		GFX_LOG_DEBUG(L"created new swapchain for test window");

		// =======================================================
		//		CREATE RENDER TARGET VIEW
		// =======================================================

		ComPtr<ID3D11Texture2D> framebuf = nullptr;
		GFX_THROW_FAILED(_dx_swap_chain->GetBuffer(
			0u,
			__uuidof(ID3D11Texture2D),
			&framebuf
		));

		GFX_THROW_FAILED(_dx_device->CreateRenderTargetView(
			framebuf.Get(),
			0u,
			&_rt_view
		));
		GFX_LOG_DEBUG(L"Created DX11 RenderTargetView.");

		// =======================================================
		//		CREATE VIEW PORT
		// =======================================================
		unsigned int vp_width  = _gfx_window.width();
		unsigned int vp_height = _gfx_window.height();		
		_viewport.TopLeftX = 0u;
		_viewport.TopLeftY = 0u;
		_viewport.Width    = vp_width;
		_viewport.Height   = vp_height;
		_viewport.MinDepth = 0u;
		_viewport.MaxDepth = 1u;


		// =======================================================
		//		CREATE VERTEX & PIXEL SHADERS
		// =======================================================

		UINT shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef DEBUG
		shader_flags |= D3DCOMPILE_DEBUG;
#endif

		ComPtr<ID3DBlob> vs_blob  = nullptr;
		ComPtr<ID3DBlob> ps_blob  = nullptr;
		ComPtr<ID3DBlob> err_blob = nullptr;

		// read from file
		GFX_THROW_FAILED(D3DReadFileToBlob(L"shaders/default_vs.cso", &vs_blob));
		// create in gpu
		GFX_THROW_FAILED(_dx_device->CreateVertexShader(
			vs_blob->GetBufferPointer(),
			vs_blob->GetBufferSize(),
			nullptr,
			&_vertex_shader
		));

		int* test = new int(4);
		delete test;
		
		// read from file
		GFX_THROW_FAILED(D3DReadFileToBlob(L"shaders/default_ps.cso", &ps_blob));
		// create in gpu
		GFX_THROW_FAILED(_dx_device->CreatePixelShader(
			ps_blob->GetBufferPointer(),
			ps_blob->GetBufferSize(),
			nullptr,
			&_pixel_shader
		));

		// =======================================================
		//		CREATE INPUT LAYOUT
		// =======================================================
		// input parameters
		const D3D11_INPUT_ELEMENT_DESC input_elements[] = {
			{"Position2", 0u, DXGI_FORMAT_R32G32_SINT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u}
		};
		// create in gpu
		GFX_THROW_FAILED(_dx_device->CreateInputLayout(
			input_elements,
			1u,
			vs_blob->GetBufferPointer(),
			vs_blob->GetBufferSize(),
			&_input_layout
		));

		// =======================================================
		//		CREATE CONSTANT BUFFER (2D VIEW MATRIX)
		// =======================================================
		// GOAL: map coordinates to pixels when zoom is at 1.0f (default)
		// Matrix for 2D Viewport
		// [ 1 0 ] -> [ 1/w  0   ]
		// [ 0 1 ] -> [ 0    1/h ]

		float zoom_scale = 1.0f;
		float view_w = _gfx_window.width();
		float view_h = _gfx_window.height();
		std::vector<float> view_scale_2d = {
			view_w,
			view_h
		};

		D3D11_BUFFER_DESC vmcbd   = {};
		vmcbd.ByteWidth           = view_scale_2d.size() * sizeof(float) * 2.0f;
		vmcbd.Usage               = D3D11_USAGE_DEFAULT;
		vmcbd.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
		vmcbd.CPUAccessFlags      = 0u;
		vmcbd.MiscFlags           = 0u;
		vmcbd.StructureByteStride = 0u;
		
		D3D11_SUBRESOURCE_DATA vmsrd = {};
		vmsrd.pSysMem = view_scale_2d.data();

		GFX_THROW_FAILED(_dx_device->CreateBuffer(&vmcbd, &vmsrd, &_view_const_buffer));

		// =======================================================
		//		CREATE VERTEX BUFFER
		// =======================================================
		dsemi::graphics::vertex_layout layout;
		layout.append("Position", dsemi::graphics::shader_data_type::SINT2);

		dsemi::graphics::vertex_array vertices(layout);
		int w = _gfx_window.width() / 2;
		int h = _gfx_window.height() / 2;
		vertices.emplace_back(0, h).emplace_back(w, -h).emplace_back(-w, -h);
		_vbuf = std::make_unique<dsemi::graphics::vertex_buffer>(_device, vertices);

		uint32_t strides = 8u;
		uint32_t offsets = 0u;
		_dx_context->VSSetShader(_vertex_shader.Get(), nullptr, 0u);
		_dx_context->PSSetShader(_pixel_shader.Get(), nullptr, 0u);
		_dx_context->IASetInputLayout(_input_layout.Get());
		_dx_context->OMSetRenderTargets(1u, _rt_view.GetAddressOf(), nullptr);
		_dx_context->RSSetViewports(1u, &_viewport);
		_dx_context->VSSetConstantBuffers(0u, 1u, _view_const_buffer.GetAddressOf());
		_vbuf->bind();
	}

	void _draw_triangle()
	{
		_dx_context->ClearRenderTargetView(_rt_view.Get(), _clear_color.as_array());
		_dx_context->OMSetRenderTargets(1u, _rt_view.GetAddressOf(), nullptr);
		_dx_context->Draw(_vbuf->get_count(), 0u);

		HRESULT hr;
		if (FAILED(hr = _dx_swap_chain->Present(0, 0)))
		{
			if (hr == DXGI_ERROR_DEVICE_REMOVED)
			{
				throw HRESULT_EXCEPTION(_dx_device->GetDeviceRemovedReason());
			}
			else
			{
				GFX_THROW_FAILED(hr);
			}
		}
	}

private:
	dsemi::graphics::device*		_device;
	ComPtr<ID3D11Device>			_dx_device;
	ComPtr<ID3D11DeviceContext>		_dx_context;
	ComPtr<IDXGISwapChain>			_dx_swap_chain;
	ComPtr<ID3D11Buffer>			_vertex_buffer;
	ComPtr<ID3D11Buffer>			_view_const_buffer;
	ComPtr<ID3D11VertexShader>		_vertex_shader;
	ComPtr<ID3D11PixelShader>		_pixel_shader;
	ComPtr<ID3D11RenderTargetView>	_rt_view;
	ComPtr<ID3D11InputLayout>		_input_layout;
	D3D11_VIEWPORT					_viewport;
	dsemi::colorf					_clear_color;

	dsemi::graphics::render_target _render_target;
	//dsemi::graphics::viewport _viewport;
	//dsemi::graphics::viewport _viewport;

	std::unique_ptr<dsemi::graphics::vertex_buffer> _vbuf;

	struct vertex
	{
		int x;
		int y;
	};
};

int main()
{
	using namespace dsemi::graphics;
	DevApp app;

	//vertex_layout layout = {
	//	{"Position", shader_data_type::FLOAT3},
	//	{"Normal", shader_data_type::FLOAT3},
	//	{"TexCoord", shader_data_type::FLOAT2},
	//};

	//vertex_array vertices(layout);
	//auto pos = { 5.0f, 5.0f, 5.0f };
	//auto nor = { 5.0f, 5.0f, 5.0f };
	//auto tex = { 5.0f, 5.0f};
	//vertices.emplace_back(
	//	pos,
	//	nor,
	//	tex
	//);

	app.init();
	app.run();

	return 0;
}
