#ifndef GRAPHICS_VERTEXBUFFER_H
#define GRAPHICS_VERTEXBUFFER_H

#include "Dsemi/graphics/api_include.h"

namespace dsemi {

	// ==================
	// Common declaration
	// ==================

	struct gfx_vertex_buffer_desc {
		void*    data_pointer = nullptr;
		uint32_t data_size    = 0u;
		uint32_t data_stride  = 0u;
	};

	class gfx_vertex_buffer {
		friend class gfx_device;
	public:
		gfx_vertex_buffer() 
			: _dx_buffer(nullptr), _stride(0)
		{}

		inline uint32_t get_stride()
		{
			return _stride;
		}

	private:
		uint32_t _stride;

	// ======================
	// DirectX 11 declaration
	// ======================
#ifdef GFX_USING_DX11
	private:
		ComPtr<ID3D11Buffer>      _dx_buffer;
#endif
	};
}

#endif