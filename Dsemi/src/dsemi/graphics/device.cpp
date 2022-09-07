#include "dspch.h"
#include "dsemi/graphics/device.h"

#include "dsemi/util/logger.hpp"

#include "dsemi/core/application.h"

namespace dsemi {

	gfx_device::~gfx_device()
	{
		delete[] _clear_color_float;
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
		_clear_color.red = 0xffffffff;
		_clear_color_float = new float[4];
		set_clear_color(0x000000ff);
	}

	void gfx_device::set_clear_color(float r, float g, float b, float a = 1.0f)
	{
		_clear_color.red      = (uint8_t)(r * 255.0f);
		_clear_color.green    = (uint8_t)(g * 255.0f);
		_clear_color.blue     = (uint8_t)(b * 255.0f);
		_clear_color.alpha    = (uint8_t)(a * 255.0f);

		_clear_color_float[0] = r;
		_clear_color_float[1] = g;
		_clear_color_float[2] = b;
		_clear_color_float[3] = a;
	}

	void gfx_device::set_clear_color(color32 rgba)
	{
		_clear_color          = rgba;

		_clear_color_float[0] = (float)rgba.red   / 255.0f;
		_clear_color_float[1] = (float)rgba.green / 255.0f;
		_clear_color_float[2] = (float)rgba.blue  / 255.0f;
		_clear_color_float[3] = (float)rgba.alpha / 255.0f;
	}

#if (defined GFX_USING_DX11)

#else

#endif

}