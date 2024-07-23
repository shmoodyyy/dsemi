#ifndef GRAPHICS_RENDER_TARGET_H
#define GRAPHICS_RENDER_TARGET_H

#include "dsemi/graphics/api_include.h"
#include "dsemi/graphics/color.h"

namespace dsemi 
{
	namespace graphics 
	{

		class render_target 
		{
			friend class device;

		public:
			render_target();
			~render_target();



			//inline ID3D11RenderTargetView const* get_view() { return _dx_render_target_view.Get(); }

		public:
			colorf clear_color;

		private:
			ComPtr<ID3D11Texture2D>        _dx_render_target;
			ComPtr<ID3D11RenderTargetView> _dx_render_target_view;
			D3D11_VIEWPORT                 _dx_view_port;
		};

		void set_render_target(render_target* target);
	}
}

#endif