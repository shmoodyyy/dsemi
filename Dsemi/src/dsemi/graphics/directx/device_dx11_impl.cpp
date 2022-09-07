#include "dspch.h"

// =========================
// DirectX 11 Implementation
// =========================

#include "dsemi/graphics/device.h"

#if (defined GFX_USING_DX11)

#include "dsemi/core/Application.h"
#include "dsemi/util/logger.hpp"

namespace dsemi {

	void gfx_device::_initialize_impl()
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

		create_swap_chain(&_active_swap_chain);

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

	IDXGIFactory* gfx_device::_get_dxgi_factory() {
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
	void gfx_device::create_vertex_buffer(gfx_vertex_buffer* vertex_buffer, gfx_vertex_buffer_desc desc)
	{
		HRESULT hr;

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage          = D3D11_USAGE_DEFAULT;
		bd.MiscFlags              = 0;
		bd.ByteWidth              = (UINT) desc.data_size;
		bd.StructureByteStride    = (UINT) desc.data_stride;
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = desc.data_pointer;

		vertex_buffer->_stride = desc.data_stride;

		GFX_THROW_FAILED(_dx_device->CreateBuffer(&bd, &sd, &vertex_buffer->_dx_buffer));
	}

	void gfx_device::create_index_buffer(gfx_index_buffer* index_buffer, gfx_index_buffer_desc desc)
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

	void gfx_device::create_vertex_shader(gfx_vertex_shader* vertex_shader, gfx_vertex_shader_desc desc)
	{
		HRESULT hr;

		GFX_THROW_FAILED(D3DReadFileToBlob(desc.path, &vertex_shader->_dx_vertex_shader_blob));
		GFX_THROW_FAILED(_dx_device->CreateVertexShader(
			vertex_shader->_dx_vertex_shader_blob->GetBufferPointer(),
			vertex_shader->_dx_vertex_shader_blob->GetBufferSize(),
			nullptr,
			&vertex_shader->_dx_vertex_shader
		));

		auto size = sizeof((D3D11_INPUT_ELEMENT_DESC*)desc.layout_elements);
		auto size2 = sizeof(D3D11_INPUT_ELEMENT_DESC);

		// Input Layout
		GFX_THROW_FAILED(_dx_device->CreateInputLayout(
			desc.layout_elements,
			desc.layout_size,
			vertex_shader->_dx_vertex_shader_blob->GetBufferPointer(),
			vertex_shader->_dx_vertex_shader_blob->GetBufferSize(),
			&vertex_shader->_input_layout._dx_input_layout
		));
	}

	void gfx_device::create_fragment_shader(gfx_fragment_shader* fragment_shader, gfx_fragment_shader_desc desc)
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

	void gfx_device::create_swap_chain(gfx_swap_chain* swap_chain)
	{
		HRESULT hr;
		// TODO: move below code to graphics_render_target
		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferDesc.Width = WND->width();
		sd.BufferDesc.Height = WND->height();
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 2;
		sd.OutputWindow = WND->hwnd();
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		GFX_THROW_FAILED(_dxgi_factory->CreateSwapChain(
			_dx_device.Get(),
			&sd,
			&swap_chain->_dxgi_swap_chain
		));
	}

	void gfx_device::create_render_target(gfx_render_target* render_target, ID3D11Resource* source)
	{
		HRESULT hr;

		if(!source)
			GFX_THROW_FAILED(_active_swap_chain._dxgi_swap_chain->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&source));

		GFX_THROW_FAILED(_dx_device->CreateRenderTargetView(
			source,
			nullptr,
			&render_target->_dx_render_target_view
		));
		source->Release();

		render_target->_dx_view_port.Width = WND->width();
		render_target->_dx_view_port.Height = WND->height();
		render_target->_dx_view_port.TopLeftX = 0u;
		render_target->_dx_view_port.TopLeftY = 0u;
		render_target->_dx_view_port.MinDepth = 0.0f;
		render_target->_dx_view_port.MaxDepth = 1.0f;
	}

	// D2D
	/*void gfx_device::create_render_target_2d()
	{

	}*/

	/*===================
	// resource binding
	=====================*/

	void gfx_device::bind_vertex_buffer(gfx_vertex_buffer* vertex_buffer)
	{
		UINT offset = 0u;
		_dx_context->IASetVertexBuffers(0u, 1u, vertex_buffer->_dx_buffer.GetAddressOf(), &vertex_buffer->_stride, &offset);
	}

	void gfx_device::bind_vertex_shader(gfx_vertex_shader* vertex_shader)
	{
		_dx_context->VSSetShader(vertex_shader->_dx_vertex_shader.Get(), nullptr, 0u);
		_dx_context->IASetInputLayout(vertex_shader->_input_layout._dx_input_layout.Get());
	}

	void gfx_device::bind_fragment_shader(gfx_fragment_shader* fragment_shader)
	{
		_dx_context->PSSetShader(fragment_shader->_dx_pixel_shader.Get(), nullptr, 0u);
	}

	void gfx_device::bind_render_target(gfx_render_target* render_target)
	{
		_dx_context->OMSetRenderTargets(1u, render_target->_dx_render_target_view.GetAddressOf(), nullptr);
		_dx_context->RSSetViewports(1u, &render_target->_dx_view_port);
		_active_render_target = render_target;
	}

	/*=============
	// draw calls
	===============*/

	void gfx_device::clear()
	{
		_dx_context->ClearRenderTargetView(_active_render_target->_dx_render_target_view.Get(), _clear_color_float);
	}

	void gfx_device::draw(unsigned int num_vertices)
	{
		_dx_context->Draw(num_vertices, 0u);
	}

	void gfx_device::draw_triangle()
	{
		_dx_context->Draw(3u, 0u);
	}

	void gfx_device::present()
	{
		HRESULT hr;

		if (FAILED(hr = _active_swap_chain._dxgi_swap_chain->Present(0, 0)))
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

}
#endif