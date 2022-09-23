#include "dspch.h"
#include "dsemi/graphics/render_context.h"

#include "dsemi/graphics/device.h"

namespace dsemi {
	namespace graphics {
		render_context::render_context(device* device)
			: _device(device)
		{
			ID3D11Device* dx_device = _device->get_dx_device();
		}

		device* render_context::get_device() const noexcept
		{
			return _device;
		}


		void render_context::set_render_target(render_target* render_target) noexcept
		{
			_render_target = render_target;
		}

		render_target* render_context::get_render_target() const noexcept
		{
			return _render_target;
		}



		/*=============
		 draw calls
		===============*/

		void render_context::clear()
		{
			_device->get_context()->ClearRenderTargetView(_render_target->_dx_render_target_view.Get(), _clear_color_float);
		}

		void render_context::draw(unsigned int num_vertices)
		{
			_dx_context->Draw(num_vertices, 0u);
		}

		void render_context::draw_triangle()
		{
			_dx_context->Draw(3u, 0u);
		}

		void render_context::present()
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
		} // void device::present()
	}
}