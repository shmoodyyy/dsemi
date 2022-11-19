#include "dspch.h"
#include "dsemi/graphics/device.h"

#include "dsemi/core/application.h"
#include "dsemi/core/window.h"
#include "dsemi/util/logger.hpp"

namespace dsemi 
{
	namespace graphics 
	{

		device::~device()
		{
			delete[] _clear_color_float;
		}

		// Goal: Create draw_triangle function that does not require _WIN32 check
		//void graphics_device::draw_triangle()
		//{
		//
		//}

		device* device::_instance = nullptr;

		void device::initialize()
		{
			ASSERT(_instance == nullptr, "Attempted to initialize the graphics API device multiple times.");
			if (!_instance)
				_instance = new device();
			logger::info("[GFX] creating graphics_device");
			_instance->_initialize_impl();
			_instance->_clear_color_float = new float[4];
			//_instance->set_clear_color(0x000000ff);
		}

		//void device::set_clear_color(float r, float g, float b, float a = 1.0f)
		//{
		//	_clear_color.red = (uint8_t)(r * 255.0f);
		//	_clear_color.green = (uint8_t)(g * 255.0f);
		//	_clear_color.blue = (uint8_t)(b * 255.0f);
		//	_clear_color.alpha = (uint8_t)(a * 255.0f);

		//	_clear_color_float[0] = r;
		//	_clear_color_float[1] = g;
		//	_clear_color_float[2] = b;
		//	_clear_color_float[3] = a;
		//}

		//void device::set_clear_color(color32 rgba)
		//{
		//	_clear_color = rgba;

		//	_clear_color_float[0] = (float)rgba.red / 255.0f;
		//	_clear_color_float[1] = (float)rgba.green / 255.0f;
		//	_clear_color_float[2] = (float)rgba.blue / 255.0f;
		//	_clear_color_float[3] = (float)rgba.alpha / 255.0f;
		//}

		void device::_initialize_impl()
		{
			// Initialize D3D
			logger::info("[DX11] initializing graphics_device in dx11 mode...");
			logger::info("[DX11] initializing direct3d...");

			HRESULT hr;
			UINT device_create_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
			device_create_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

			logger::info("[DX11] creating d3d11device...");
			GFX_THROW_FAILED(D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				device_create_flags,
				nullptr,
				0,
				D3D11_SDK_VERSION,
				&_dx_device,
				nullptr,
				&_dx_context
			));

			_dxgi_factory = _get_dxgi_factory();

			//create_swap_chain(&_active_swap_chain);

			_dx_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			D3D11_RASTERIZER_DESC rd;
			rd.FillMode = D3D11_FILL_WIREFRAME;
			rd.CullMode = D3D11_CULL_BACK;
			rd.FrontCounterClockwise = FALSE;
			rd.DepthBias = 0u;
			rd.DepthBiasClamp = 0.0f;
			rd.SlopeScaledDepthBias = 0.0f;
			rd.DepthClipEnable = TRUE;
			rd.ScissorEnable = FALSE;
			rd.MultisampleEnable = FALSE;
			rd.AntialiasedLineEnable = FALSE;

			GFX_THROW_FAILED(_dx_device->CreateRasterizerState(&rd, &_dx_rasterizer_state));

			logger::info("[DX11] finished initializing direct3d.");
			logger::info("[DX11] graphics_device finished initializing.");
		}

		IDXGIFactory* device::_get_dxgi_factory() {
			HRESULT hr;
			// DirectX on some srs bs
			// the way i understand it is:
			// we get the IDXGI interface of our created d3d device,
			IDXGIDevice* dxgi_device;
			GFX_THROW_FAILED(_dx_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgi_device));
			// then we go up the tree of hardware API interfaces,
			IDXGIAdapter* dxgi_adapter;
			GFX_THROW_FAILED(dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgi_adapter));
			// until we find the factory responsible for creating the adapter/device?
			IDXGIFactory* dxgi_factory;
			GFX_THROW_FAILED(dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgi_factory));

			return dxgi_factory;
		}

		/*====================
		// resource creation
		======================*/
		// D3D
		void device::create_vertex_buffer(vertex_buffer* vertex_buffer)
		{
			HRESULT hr;

			D3D11_BUFFER_DESC bd = {};
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.MiscFlags = 0;
			bd.ByteWidth = (UINT)0u;
			//bd.StructureByteStride = (UINT)desc.data_stride;
			D3D11_SUBRESOURCE_DATA sd = {};
			//sd.pSysMem = desc.data_pointer;

			//vertex_buffer->_stride = desc.data_stride;

			//GFX_THROW_FAILED(_dx_device->CreateBuffer(&bd, &sd, &vertex_buffer->_dx_buffer));
		}

		void device::create_index_buffer(gfx_index_buffer* index_buffer, index_buffer_desc desc)
		{
			HRESULT hr;

			D3D11_BUFFER_DESC bd = {};
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.MiscFlags = 0;
			bd.ByteWidth = (UINT)desc.data_size;
			bd.StructureByteStride = (UINT)desc.data_stride;
			D3D11_SUBRESOURCE_DATA sd = {};
			sd.pSysMem = desc.data_pointer;

			GFX_THROW_FAILED(_dx_device->CreateBuffer(&bd, &sd, &index_buffer->_dx_buffer));
		}

		void device::create_vertex_shader(vertex_shader* vertex_shader, vertex_shader_desc desc)
		{
			HRESULT hr;

			GFX_THROW_FAILED(D3DReadFileToBlob(desc.path, &vertex_shader->_dx_vertex_shader_blob));
			GFX_THROW_FAILED(_dx_device->CreateVertexShader(
				vertex_shader->_dx_vertex_shader_blob->GetBufferPointer(),
				vertex_shader->_dx_vertex_shader_blob->GetBufferSize(),
				nullptr,
				&vertex_shader->_dx_vertex_shader
			));

			//auto size = sizeof((D3D11_INPUT_ELEMENT_DESC*)desc.layout_elements);
			auto size2 = sizeof(D3D11_INPUT_ELEMENT_DESC);

			// Input Layout
			/*GFX_THROW_FAILED(_dx_device->CreateInputLayout(
				desc.layout_elements,
				desc.layout_size,
				vertex_shader->_dx_vertex_shader_blob->GetBufferPointer(),
				vertex_shader->_dx_vertex_shader_blob->GetBufferSize(),
				&vertex_shader->_input_layout._dx_input_layout
			));*/
		}

		void device::create_fragment_shader(fragment_shader* fragment_shader, fragment_shader_desc desc)
		{
			HRESULT hr;

			GFX_THROW_FAILED(D3DReadFileToBlob(desc.path, &fragment_shader->_dx_pixel_shader_blob));
			GFX_THROW_FAILED(_dx_device->CreatePixelShader(
				fragment_shader->_dx_pixel_shader_blob->GetBufferPointer(),
				fragment_shader->_dx_pixel_shader_blob->GetBufferSize(),
				nullptr,
				&fragment_shader->_dx_pixel_shader
			));
		}

		//void device::create_swap_chain(swap_chain* swap_chain, window* window)
		//{
		//	HRESULT hr;
		//	// TODO: move below code to graphics_render_target
		//	DXGI_SWAP_CHAIN_DESC sd = {};
		//	sd.BufferDesc.Width = window->width();
		//	sd.BufferDesc.Height = window->height();
		//	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		//	sd.BufferDesc.RefreshRate.Numerator = 0;
		//	sd.BufferDesc.RefreshRate.Denominator = 0;
		//	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		//	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		//	sd.SampleDesc.Count = 1;
		//	sd.SampleDesc.Quality = 0;
		//	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		//	sd.BufferCount = 2;
		//	sd.OutputWindow = window->hwnd();
		//	sd.Windowed = TRUE;
		//	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		//	GFX_THROW_FAILED(_dxgi_factory->CreateSwapChain(
		//		_dx_device.Get(),
		//		&sd,
		//		&swap_chain->_dxgi_swap_chain
		//	));
		//}

		void device::create_render_target(render_target* render_target, ID3D11Resource* source)
		{
			HRESULT hr;

			//if (!source)
			//	GFX_THROW_FAILED(_active_swap_chain._dxgi_swap_chain->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&source));

			GFX_THROW_FAILED(_dx_device->CreateRenderTargetView(
				source,
				nullptr,
				&render_target->_dx_render_target_view
			));
			source->Release();

			//render_target->_dx_view_port.Width = window->width();
			//render_target->_dx_view_port.Height = window->height();
			render_target->_dx_view_port.TopLeftX = 0u;
			render_target->_dx_view_port.TopLeftY = 0u;
			render_target->_dx_view_port.MinDepth = 0.0f;
			render_target->_dx_view_port.MaxDepth = 1.0f;
		}

		void device::bind_vertex_buffer(const vertex_buffer* vertex_buffer)
		{
			UINT offset = 0u;
			_dx_context->IASetVertexBuffers(0u, 1u, vertex_buffer->get_dx_buffer().GetAddressOf(), &vertex_buffer->get_stride(), &offset);
		}

		void device::bind_vertex_shader(vertex_shader* vertex_shader)
		{
			_dx_context->VSSetShader(vertex_shader->_dx_vertex_shader.Get(), nullptr, 0u);
			//_dx_context->IASetInputLayout(vertex_shader->_input_layout._dx_input_layout.Get());
		}

		void device::bind_fragment_shader(fragment_shader* fragment_shader)
		{
			_dx_context->PSSetShader(fragment_shader->_dx_pixel_shader.Get(), nullptr, 0u);
		}

		void device::bind_render_target(render_target* render_target)
		{
			_dx_context->OMSetRenderTargets(1u, render_target->_dx_render_target_view.GetAddressOf(), nullptr);
			_dx_context->RSSetViewports(1u, &render_target->_dx_view_port);
			//_active_render_target = render_target;
		}

	} // namespace graphics
} // namespace dsemi