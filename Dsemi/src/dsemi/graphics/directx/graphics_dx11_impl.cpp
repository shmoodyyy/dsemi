#include "dspch.h"
#include "Dsemi/graphics/graphics.h"

namespace dsemi {

	void gfx_create_vertex_buffer(void* data, size_t byte_stride)
	{
		gfx_vertex_buffer dx_vertex_buffer();
		
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.ByteWidth = sizeof(data);
		bd.StructureByteStride = sizeof(byte_stride);
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = data;
		//GFX_THROW_FAILED(_dx_device->CreateBuffer(&bd, &sd, &_dx_buffer));
		//_dx_buffer_stride = byte_stride;
	}

}