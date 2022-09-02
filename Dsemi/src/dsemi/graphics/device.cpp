#include "dspch.h"
#include "dsemi/graphics/device.h"

#include "dsemi/util/logger.hpp"

#include "dsemi/core/application.h"

namespace dsemi {

	gfx_device::~gfx_device()
	{
	}

	// Goal: Create draw_triangle function that does not require _WIN32 check
	//void graphics_device::draw_triangle()
	//{
	//
	//}

	void gfx_device::initialize()
	{
		logger::info("[GFX] creating graphics_device");
		_initialize_impl();
		_clear_colour[0] = 0.2f; // R
		_clear_colour[1] = 0.2f; // G
		_clear_colour[2] = 0.2f; // B
		_clear_colour[3] = 1.0f; // A
	}

	/*===================
	// state management
	=====================*/
	void gfx_device::set_clear_colour(float r, float g, float b)
	{
		_clear_colour[0] = r; // R
		_clear_colour[1] = g; // G
		_clear_colour[2] = b; // B
	}

#if (defined GFX_USING_DX11)

#else

#endif

}