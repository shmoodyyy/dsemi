#include "dspch.h"
#include "dsemi/graphics/render_target.h"

namespace dsemi {
	namespace graphics {

		render_target::render_target()
		{
		}

		ID3D11RenderTargetView* render_target::get_view() noexcept
		{
			return _dx_render_target_view.Get();
		}
	}
}