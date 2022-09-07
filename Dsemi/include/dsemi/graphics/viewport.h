#ifndef HEADER_DSEMI_GRAPHICS_VIEWPORT
#define HEADER_DSEMI_GRAPHICS_VIEWPORT

#include "api_include.h"

namespace dsemi {
	class viewport
	{
	public:
		viewport();
		~viewport();



#ifdef GFX_USING_DX11
	private:
		D3D11_VIEWPORT _dx_viewport;

#endif

	};
}

#endif