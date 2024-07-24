#include "dsemi/graphics/render_context.h"

#include "dsemi/graphics/render_target.h"
#include "dsemi/graphics/device.h"

namespace dsemi {
	namespace graphics {
		render_context::render_context(Device* device)
			: _device(device)
		{
			ID3D11Device* dx_device = _device->get_dx_device();
		}

		Device* render_context::get_device() const
		{
			return _device;
		}


		void render_context::set_render_target(render_target* render_target)
		{
			_render_target = render_target;
		}

		//render_target* render_context::get_render_target() const
		//{
		//	return _render_target;
		//}



		/*=============
		 draw calls
		===============*/

		void render_context::clear()
		{
			//get_dx_context()->ClearRenderTargetView(_render_target->_dx_render_target_view.Get(), _clear_color_float);
		}

		void render_context::draw(unsigned int num_vertices)
		{
			get_dx_context()->Draw(num_vertices, 0u);
		}

		void render_context::draw_triangle()
		{
			get_dx_context()->Draw(3u, 0u);
		}

		void render_context::present()
		{
			HRESULT hr;

			/*if (FAILED(hr = _active_swap_chain._dxgi_swap_chain->Present(0, 0)))
			{
				if (hr == DXGI_ERROR_DEVICE_REMOVED)
				{
					throw HRESULT_EXCEPTION(_dx_device->GetDeviceRemovedReason());
				}
				else
				{
					GFX_THROW_FAILED(hr);
				}
			}*/
		} // void device::present()
		ID3D11DeviceContext* render_context::get_dx_context() const
		{
			return _device->get_context();
		}
	}
}
