#ifndef GRAPHICS_INPUT_LAYOUT_H
#define GRAPHICS_INPUT_LAYOUT_H

#include "Dsemi/graphics/api_include.h"

namespace dsemi 
{
	namespace graphics
	{

	}

	class gfx_input_layout {
		friend class gfx_device;
	public:
		gfx_input_layout()
			: _dx_input_layout(nullptr)
		{}

	private:
		ComPtr<ID3D11InputLayout> _dx_input_layout;
	};
}

#endif