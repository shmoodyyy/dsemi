#ifndef GRAPHICS_INPUT_LAYOUT_H
#define GRAPHICS_INPUT_LAYOUT_H

#include "Dsemi/graphics/api_include.h"

namespace dsemi {

#if (defined GFX_USING_DX11)
	typedef D3D11_INPUT_ELEMENT_DESC gfx_input_element;
#endif

	class gfx_input_layout {
		friend class gfx_device;
	public:
		gfx_input_layout()
			: _dx_input_layout(nullptr)
		{}

#ifdef GFX_USING_DX11
	private:
		ComPtr<ID3D11InputLayout> _dx_input_layout;
	};
#endif
}

#endif