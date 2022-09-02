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

	/*=========================================================================================
	//
	//	Singleton/Static class managing the entire graphics framework objects and memory
	//	- using singleton implementation to avoid having to use global/static variables
	//
	===========================================================================================*/

	class gfx_core {
	public:
		static gfx_core* instance();
		static void initialize();

		static gfx_device* get_device();

		// Static members
	private:
		static gfx_core* _instance;

		// Instance functions
	private:
		void _initialize_instance();

		// Instance members
	private:
		gfx_device _device;

	};

	/*================================
	//
	//	Graphics Resource Management
	//
	==================================*/

	gfx_device* gfx_get_device();

	//void gfx_create_buffer(gfx_buffer* buffer, gfx_buffer_desc desc);
	//void gfx_create_vertex_buffer(gfx_buffer* buffer, void* vertices);

	/*================================
	//
	//	Low-Level API Access
	//  - to be scrapped since it's just a layer of abstraction for no benefit (rewriting DX11 code with no real abstraction depth)
	//  - actually no since im gonna be needing to have vulkan and directx implemented based on platform, and directx12 is very hands-on
	//
	==================================*/

	/* Resource Creation */
	void gfx_create_vertex_buffer   (gfx_vertex_buffer* buffer, gfx_vertex_buffer_desc desc);
	void gfx_create_input_layout    (gfx_input_layout* layout, gfx_input_element* layout_elements);
	void gfx_create_vertex_shader   (gfx_vertex_shader* shader, gfx_vertex_shader_desc desc);
	void gfx_create_fragment_shader (gfx_fragment_shader* shader, gfx_fragment_shader_desc desc);

	/* Resource Binding */
	/*void gfx_bind_vertex_buffer(gfx_vertex_buffer* buffer);
	void gfx_bind_input_layout(gfx_input_layout* )*/

	/* Draw Calls */
	//void gfx_draw_call (unsigned int vertex_count);

	inline void gfx_present()
	{
		gfx_core::get_device()->present();
	}

	/*=================================
	//
	//	Immediate Rendering Interface
	//
	===================================*/

	// Graphics State Management
	void gfx_set_colour(vector3f rgb);
	void gfx_set_clear_colour(vector3f rgb);

	// Render calls

	void gfx_clear();
	void gfx_render2d_fill_rect(vector2i pos, vector2i dimensions);
	void gfx_render2d_fill_poly();
	void gfx_render2d_fill_circle(vector2f pos, float radius);
	void gfx_render2d_fill_triangle();
	void gfx_present();

	/*================================
	//
	//	Pipeline Interface
	//
	==================================*/



}

#endif