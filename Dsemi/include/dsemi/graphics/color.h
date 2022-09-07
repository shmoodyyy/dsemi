#ifndef GRAPHICS_COLOUR_H
#define GRAPHICS_COLOUR_H

#include <stdint.h>
#include "dsemi/util/assert.h"

namespace dsemi {
	struct color32 
	{
		color32(int rgba)
			: red   (( rgba & 0xff000000 ) >> 24 )
			, green (( rgba & 0x00ff0000 ) >> 16 )
			, blue  (( rgba & 0x0000ff00 ) >>  8 )
			, alpha (( rgba & 0x000000ff )       )
		{}
		color32(uint8_t r = 0xff, uint8_t g = 0xff, uint8_t b = 0xff, uint8_t a = 0xff)
			: red   (r)
			, green (g)
			, blue  (b)
			, alpha (a)
		{}

		uint8_t red;
		uint8_t green;
		uint8_t blue;
		uint8_t alpha;

		inline int rgba() const noexcept { return ( ((int)red) << 24) + ( ((int)green) << 16) + ( ((int)blue) << 8) + ((int)alpha); }
		inline void rgba(int val) noexcept {
			red   = (val & 0xff000000) >> 24;
			green = (val & 0x00ff0000) >> 16;
			blue  = (val & 0x0000ff00) >>  8;
			alpha = (val & 0x000000ff);
		}
	};
}

#endif