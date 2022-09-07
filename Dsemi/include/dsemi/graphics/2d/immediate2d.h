#ifndef GRAPHICS_IMMEDIATE2D_H
#define GRAPHICS_IMMEDIATE2D_H

#include "dsemi/graphics/render_target.h"

namespace dsemi {
	namespace immediate2d {
		gfx_render_target* active_render_target;

		void set_render_target();

		void draw_rect();
		void draw_line();
		void draw_circle();

		void present();
		void flush();
	}
}

#endif