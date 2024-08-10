#include <dsemi/graphics/device.h>
#include <dsemi/core/application.h>
#include <dsemi/core/window.h>
#include <dsemi/util/logger.h>

namespace dsemi
{
	namespace graphics 
	{
        Device::Device()
        {
			GFX_LOG_DEBUG(L"creating graphics::device");
			_initialize_impl();
			_clear_color_float = new float[4];
			//_instance->set_clear_color(0x000000ff);
        }

		Device::~Device()
		{
			delete[] _clear_color_float;
		}

		// Goal: Create draw_triangle function that does not require _WIN32 check
		//void graphics_device::draw_triangle()
		//{
		//
		//}

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

		void Device::_initialize_impl()
		{
			// Initialize D3D
			GFX_LOG_DEBUG(L"initializing graphics::device in dx11 mode...");
			GFX_LOG_DEBUG(L"initializing direct3d...");

			HRESULT hr;
			UINT device_create_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifndef NDEBUG
			device_create_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

			GFX_LOG_DEBUG(L"creating d3d11device...");
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

			GFX_LOG_DEBUG(L"finished initializing direct3d.");
			GFX_LOG_DEBUG(L"graphics::device finished initializing.");
		}

		IDXGIFactory* Device::_get_dxgi_factory() {
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

			GFX_LOG_DEBUG(L"initialized graphics::device::_dxgi_factory");

			return dxgi_factory;
		}
	}
}
