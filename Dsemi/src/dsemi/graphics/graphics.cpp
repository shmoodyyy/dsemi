#include "dspch.h"
#include "dsemi/graphics/graphics.h"

#include "dsemi/util/logger.hpp"

namespace dsemi {

	gfx_device* gfx_get_device()
	{
		return gfx_core::get_device();
	}

	/*void gfx_create_vertex_buffer(gfx_buffer* buffer, void* vertices)
	{

	}*/

	/*===================================
	//	singleton/static implementation
	=====================================*/

	gfx_core* gfx_core::_instance = nullptr;

	gfx_core* gfx_core::instance()
	{
		return _instance;
	}

	void gfx_core::initialize()
	{
		if (_instance)
		{
			logger::warn("[GFX] attempted to reinitialize graphics");
			return;
		}

		logger::info("[GFX_CORE] creating gfx_core instance");
		_instance = new gfx_core();
		_instance->_initialize_instance();
	}

	void gfx_core::_initialize_instance()
	{
		logger::info("[GFX_CORE] initializing gfx_core instance");
		_device.initialize();
	}

	gfx_device* gfx_core::get_device()
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

	void gfx_set_render_target(gfx_render_target* render_target)
	{
		gfx_get_device()->bind_render_target(render_target);
	}

	// Render Calls

	void gfx_render2d_fill_circle(vector2f pos, float radius)
	{

	}
}