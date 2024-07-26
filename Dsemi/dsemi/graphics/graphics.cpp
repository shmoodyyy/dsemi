#include "dsemi/graphics/graphics.h"
#include "dsemi/util/logger.h"

namespace dsemi {
	namespace graphics {
		/*void gfx_create_vertex_buffer(gfx_buffer* buffer, void* vertices)
		{

		}*/

		/*===================================
		//	singleton/static implementation
		=====================================*/

		/*================================
		//
		//	Graphics Resource Management
		//
		==================================*/



		/*================================
		//
		//	Low-Level API Access
		//  - to be scrapped since it's just a layer of abstraction for no benefit (rewriting DX11 code with no real abstraction depth)
		//  - actually no since im gonna be needing to have vulkan and directx implemented based on platform, and directx12 is very hands-on
		//
		==================================*/

		/*=================================
		//
		//	Immediate Rendering Interface
		//
		===================================*/

		// Graphics State Management
		void set_render_target(RenderTarget* render_target)
		{
		}

		// Render Calls
		void render2d_fill_circle(Vector2f pos, float radius)
		{
		}
	}
}
