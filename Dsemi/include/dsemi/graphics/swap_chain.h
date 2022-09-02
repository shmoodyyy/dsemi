#ifndef GRAPHICS_SWAPCHAIN_H
#define GRAPHICS_SWAPCHAIN_H

#include "Dsemi/graphics/api_include.h"

namespace dsemi {

	class gfx_swap_chain {
		friend class gfx_device;
	public:
		gfx_swap_chain() {}

#if (defined GFX_USING_DX11)
	private:
		ComPtr<IDXGISwapChain> _dxgi_swap_chain;
		ComPtr<ID3D11Resource> _dxgi_swap_chain_back_buffer;

#endif
	};

}

#endif