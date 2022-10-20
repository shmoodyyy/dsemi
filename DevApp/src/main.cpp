#include <dsemi.h>
#include <iostream>
#include "scenes/testscene2d.h"
#include <dsemi/graphics/color.h>
#include <dsemi/graphics/api_include.h>

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
		_gfx_window.create(640, 480, L"GFX Rendering Test Window");

		_init_directx();
	}

	virtual void on_event(dsemi::ievent& e) override
	{
		dsemi::event_dispatcher dispatcher(e);
		dispatcher.dispatch<dsemi::window_close_event>(BIND_EVENT(DevApp::_on_window_close));
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

private:
	test_scene2d _test_scene;
	dsemi::window _gfx_window;

private:

	void _init_directx()
	{
		// =======================================================
		// 
		//		CREATE DEVICE AND SWAP CHAIN
		// 
		// =======================================================
		UINT device_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef DEBUG
		device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferDesc.Width                   = _gfx_window.width();
		sd.BufferDesc.Height                  = _gfx_window.height();
		sd.BufferDesc.Format                  = DXGI_FORMAT_B8G8R8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator   = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.SampleDesc.Count                   = 1u;
		sd.SampleDesc.Quality                 = 0u;
		sd.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount                        = 2u;
		sd.OutputWindow                       = _gfx_window.get_hwnd();
		sd.Windowed                           = TRUE;
		sd.SwapEffect                         = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		HRESULT hr;
		D3D_FEATURE_LEVEL feature_levels;
		GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			device_flags,
			nullptr,
			0u,
			D3D11_SDK_VERSION,
			&sd,
			&_dx_swap_chain,
			&_dx_device,
			&feature_levels,
			&_dx_context
		));
		_dx_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		std::stringstream sstream;
		dsemi::logger::info("Created DirectX11 Device and Swapchain.");
		sstream << "DX11 selected feature level: " << feature_levels;
		dsemi::logger::info(sstream.str());


		// =======================================================
		// 
		//		CREATE RENDER TARGET VIEW
		// 
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
		dsemi::logger::info("Created DX11 RenderTargetView.");

		// =======================================================
		// 
		//		CREATE VIEW PORT
		// 
		// =======================================================

		_viewport = {};
		_viewport.TopLeftX = 0u;
		_viewport.TopLeftY = 0u;
		_viewport.Width    = _gfx_window.width();
		_viewport.Height   = _gfx_window.height();
		_viewport.MinDepth = 0u;
		_viewport.MaxDepth = 1u;

		// =======================================================
		// 
		//		CREATE VERTEX & PIXEL SHADERS
		// 
		// =======================================================

		UINT shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef DEBUG
		shader_flags |= D3DCOMPILE_DEBUG;
#endif

		ComPtr<ID3DBlob> vs_blob  = nullptr;
		ComPtr<ID3DBlob> ps_blob  = nullptr;
		ComPtr<ID3DBlob> err_blob = nullptr;

		GFX_THROW_FAILED(D3DReadFileToBlob(L"shaders/default_vs.cso", &vs_blob));
		GFX_THROW_FAILED(_dx_device->CreateVertexShader(
			vs_blob->GetBufferPointer(),
			vs_blob->GetBufferSize(),
			nullptr,
			&_vertex_shader
		));

		GFX_THROW_FAILED(D3DReadFileToBlob(L"shaders/default_ps.cso", &ps_blob));
		GFX_THROW_FAILED(_dx_device->CreatePixelShader(
			ps_blob->GetBufferPointer(),
			ps_blob->GetBufferSize(),
			nullptr,
			&_pixel_shader
		));

		// =======================================================
		// 
		//		CREATE INPUT LAYOUT
		// 
		// =======================================================

		const D3D11_INPUT_ELEMENT_DESC input_elements[] = {
			{"Position", 0u, DXGI_FORMAT_R32G32_SINT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u}
		};
		GFX_THROW_FAILED(_dx_device->CreateInputLayout(
			input_elements,
			1u,
			vs_blob->GetBufferPointer(),
			vs_blob->GetBufferSize(),
			&_input_layout
		));

		// =======================================================
		// 
		//		CREATE VERTEX BUFFER
		// 
		// =======================================================

		_vertices = {
			{0, 1},
			{1, -1},
			{-1, -1},
		};

		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth           = _vertices.size() * sizeof(int) * 2u;
		bd.Usage               = D3D11_USAGE_DEFAULT;
		bd.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags      = 0u;
		bd.MiscFlags           = 0u;
		bd.StructureByteStride = sizeof(int) * 2u;
		D3D11_SUBRESOURCE_DATA bsrd = {};
		bsrd.pSysMem = _vertices.data();

		GFX_THROW_FAILED(_dx_device->CreateBuffer(
			&bd,
			&bsrd,
			&_vertex_buffer
		));
		dsemi::logger::info("Created DX11 Vertex Buffer.");

		_dx_context->RSSetViewports(1u, &_viewport);
		_dx_context->VSSetShader(_vertex_shader.Get(), nullptr, 0u);
		_dx_context->PSSetShader(_pixel_shader.Get(), nullptr, 0u);
		_dx_context->IASetInputLayout(_input_layout.Get());
	}

	void _draw_triangle()
	{
		_dx_context->ClearRenderTargetView(_rt_view.Get(), _clear_color);
		_dx_context->OMSetRenderTargets(1u, _rt_view.GetAddressOf(), nullptr);

		UINT strides = sizeof(int) * 2;
		UINT offsets = 0u;
		_dx_context->IASetVertexBuffers(
			0u,
			1u,
			_vertex_buffer.GetAddressOf(),
			&strides,
			&offsets
		);


		_dx_context->Draw(_vertices.size(), 0u);

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
	ComPtr<ID3D11Device> _dx_device;
	ComPtr<ID3D11DeviceContext> _dx_context;
	ComPtr<IDXGISwapChain> _dx_swap_chain;

	ComPtr<ID3D11Buffer> _vertex_buffer;

	ComPtr<ID3D11VertexShader> _vertex_shader;
	ComPtr<ID3D11PixelShader> _pixel_shader;
	ComPtr<ID3D11RenderTargetView> _rt_view;
	ComPtr<ID3D11InputLayout> _input_layout;
	D3D11_VIEWPORT _viewport;
	float _clear_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};

	struct vertex
	{
		int x;
		int y;
	};
	std::vector<vertex> _vertices;
};

int main()
{
	using namespace dsemi::graphics;
	DevApp app;

	vertex_layout layout = {
		{"Position", shader_data_type::FLOAT3},
		{"Normal", shader_data_type::FLOAT3},
		{"TexCoord", shader_data_type::FLOAT2},
	};

	vertex_array vertices(layout);
	auto pos = { 5.0f, 5.0f, 5.0f };
	auto nor = { 5.0f, 5.0f, 5.0f };
	auto tex = { 5.0f, 5.0f};
	vertices.emplace_back(
		pos,
		nor,
		tex
	);

	app.init();
	app.run();

	return 0;
}