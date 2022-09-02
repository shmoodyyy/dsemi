#include "dspch.h"
#include "dsemi/graphics/colour.h"

namespace dsemi {

	colour_rgba::colour_rgba()
		: r(0), g(0), b(0), a(255)
	{
	}

	colour_rgba::colour_rgba(uint32_t rgba)
		: r(rgba >> 6), g((rgba << 2) >> 6), b((rgba << 4) >> 6), a ((rgba << 6) >> 6)
	{
	}

	colour_rgba::colour_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		: r(r), g(g), b(b), a(a)
	{
	}

}