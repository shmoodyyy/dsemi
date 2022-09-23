#ifndef GRAPHICS_RENDER_TARGET_H
#define GRAPHICS_RENDER_TARGET_H

#include "dsemi/graphics/api_include.h"
#include "dsemi/graphics/color.h"

namespace dsemi {
	namespace graphics {
		class render_target {
			friend class device;
		public:
			render_target();

			ID3D11RenderTargetView* get_view() noexcept;

		protected:
			ComPtr<ID3D11Texture2D>        _dx_render_target;
			ComPtr<ID3D11RenderTargetView> _dx_render_target_view;
			D3D11_VIEWPORT                 _dx_view_port;

			float* _clear_color_float;
		};
	}
}

#endif