#ifndef GRAPHICS_COLOUR_H
#define GRAPHICS_COLOUR_H

#include <stdint.h>
#include "dsemi/util/assert.h"

namespace dsemi {
	struct color32
	{
		color32(int rgba)
			: red	((rgba & 0xff000000) >> 24)
			, green	((rgba & 0x00ff0000) >> 16)
			, blue	((rgba & 0x0000ff00) >> 8)
			, alpha	((rgba & 0x000000ff))
		{}
		color32(uint8_t r = 0xff, uint8_t g = 0xff, uint8_t b = 0xff, uint8_t a = 0xff)
			: red	(r)
			, green	(g)
			, blue	(b)
			, alpha	(a)
		{}

		uint8_t red;
		uint8_t green;
		uint8_t blue;
		uint8_t alpha;

		inline unsigned int get_rgba() const { return *(int*)(&red); }
		inline void         set_rgba(unsigned int val) {
			red		= (val & 0xff000000) >> 24;
			green	= (val & 0x00ff0000) >> 16;
			blue	= (val & 0x0000ff00) >> 8;
			alpha	= (val & 0x000000ff);
		}
	};

	struct colorf
	{
		colorf(float set_all = 1.0f)
			:red(set_all)
			,green(set_all)
			,blue(set_all)
			,alpha(1.0f)
		{}

		colorf(float r, float g, float b, float a = 1.0f)
			:red(r)
			,green(g)
			,blue(b)
			,alpha(a)
		{}

		float red;
		float green;
		float blue;
		float alpha;

		// exclusively for DirectX calls that require an array of 4 floats
		inline float* as_array() { return &red; }
	};
}

#endif