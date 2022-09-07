#ifndef GRAPHICS_RENDER_TARGET_H
#define GRAPHICS_RENDER_TARGET_H

#include "dsemi/graphics/api_include.h"
#include "dsemi/graphics/color.h"

namespace dsemi {

	class gfx_render_target {
		friend class gfx_device;
	public:
		gfx_render_target();

	protected:
		color32 clear_color;

#if (defined GFX_USING_DX11)
	private:
		ComPtr<ID3D11RenderTargetView> _dx_render_target_view;
		ComPtr<IDXGISwapChain>         _dx_swap_chain;
		D3D11_VIEWPORT                 _dx_view_port;

		float* _clear_color_float;
#endif
	};
}

#endif