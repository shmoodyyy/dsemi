#ifndef GRAPHICS_COLOUR_H
#define GRAPHICS_COLOUR_H

namespace dsemi {

	struct colour_rgba {
		colour_rgba();
		colour_rgba(uint32_t rgba);
		colour_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	};

}

#endif