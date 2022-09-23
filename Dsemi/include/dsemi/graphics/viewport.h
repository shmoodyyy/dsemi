#ifndef HEADER_DSEMI_GRAPHICS_VIEWPORT
#define HEADER_DSEMI_GRAPHICS_VIEWPORT

#include "api_include.h"

namespace dsemi {
	namespace graphics
	{
		class viewport
		{
		public:
			viewport();
			~viewport();


		private:
			D3D11_VIEWPORT _dx_viewport;

		};
	}
}

#endif