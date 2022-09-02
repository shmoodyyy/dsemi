#include "dspch.h"
#include "dsemi/graphics/render_target.h"

namespace dsemi {

	gfx_render_target::gfx_render_target()
	{
#ifdef GFX_USING_DX11
		_dx_render_target_view = nullptr;

#elif


#endif
	}

}