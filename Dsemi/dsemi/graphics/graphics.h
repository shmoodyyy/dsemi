#ifndef GRAPHICS_H
#define GRAPHICS_H

/*================================================
//  Main Include file for the Graphics Framework
//  
//  to be elaborated.
==================================================*/

#include "Dsemi/graphics/device.h"
#include "Dsemi/Math/vector3f.h"
#include "Dsemi/Math/vector2.h"

namespace dsemi {
	namespace graphics {
		/*=========================================================================================
		//
		//	Singleton/Static class managing the entire graphics framework objects and memory
		//	- using singleton implementation to avoid having to use global/static variables
		//
		===========================================================================================*/

		/*================================
		//
		//	Graphics Resource Management
		//
		==================================*/

		//void gfx_create_buffer(gfx_buffer* buffer, gfx_buffer_desc desc);
		//void gfx_create_vertex_buffer(gfx_buffer* buffer, void* vertices);

		/*================================
		//
		//	Low-Level API Access
		//  - to be scrapped since it's just a layer of abstraction for no benefit (rewriting DX11 code with no real abstraction depth)
		//  - actually no since im gonna be needing to have vulkan and directx implemented based on platform, and directx12 is very hands-on
		//
		==================================*/

		/* Resource Binding */
		/*void gfx_bind_vertex_buffer(gfx_vertex_buffer* buffer);
		void gfx_bind_input_layout(input_layout* )*/

		/* Draw Calls */
		//void gfx_draw_call (unsigned int vertex_count);

		inline void present()
		{
			//core::get_device()->present();
		}

		/*=================================
		//
		//	Immediate Rendering Interface
		//
		===================================*/

		// Graphics State Management
		//void set_colour(vector3f rgb);
		//void set_clear_colour(vector3f rgb);

		// Render calls

		void clear();
		//void render2d_fill_rect(vector2i pos, vector2i dimensions);
		void render2d_fill_poly();
		//void render2d_fill_circle(Vector2f pos, float radius);
		void render2d_fill_triangle();
		void present();

		/*================================
		//
		//	Pipeline Interface
		//
		==================================*/
	}
}

#endif
