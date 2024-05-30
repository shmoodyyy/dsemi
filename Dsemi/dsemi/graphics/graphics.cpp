#include "dsemi/graphics/graphics.h"

#include "dsemi/util/logger.h"

namespace dsemi {
	namespace graphics {
		device* get_device()
		{
			return core::get_device();
		}

		/*void gfx_create_vertex_buffer(gfx_buffer* buffer, void* vertices)
		{

		}*/

		/*===================================
		//	singleton/static implementation
		=====================================*/

		core* core::_instance = nullptr;

		core* core::get()
		{
			return _instance;
		}

		void core::initialize()
		{
			if (_instance)
			{
				logger::warn(L"[GFX] attempted to reinitialize graphics");
				return;
			}

			logger::info(L"[GFX_CORE] creating gfx_core instance");
			_instance = new core();
			_instance->_initialize_instance();
		}

		void core::_initialize_instance()
		{
			logger::info(L"[GFX_CORE] initializing gfx_core instance");
			_device.initialize();
		}

		device* core::get_device()
		{
			return &_instance->_device;
		}

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

		void set_render_target(render_target* render_target)
		{
			get_device()->bind_render_target(render_target);
		}

		// Render Calls

		void render2d_fill_circle(vector2f pos, float radius)
		{

		}
	}
}
