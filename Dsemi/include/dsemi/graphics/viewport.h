#ifndef HEADER_DSEMI_GRAPHICS_VIEWPORT
#define HEADER_DSEMI_GRAPHICS_VIEWPORT

#include "dsemi/graphics/api_include.h"
#include "dsemi/math/vector2.h"

namespace dsemi {
	namespace graphics
	{
		class viewport
		{
		public:
			viewport();
			~viewport();



			vector2i position;
			vector2i size;
		private:
			D3D11_VIEWPORT _dx_viewport;
		};
	}
}

#endif