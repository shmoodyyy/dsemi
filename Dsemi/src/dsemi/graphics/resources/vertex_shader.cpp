#include "dspch.h"
#include "Dsemi/graphics/resources/vertex_shader.h"

namespace dsemi {
	namespace graphics {

#if (defined GFX_USING_DX11)

		void* vertex_shader::byte_code()
		{
			return _dx_vertex_shader_blob->GetBufferPointer();
		}

		size_t vertex_shader::byte_code_size()
		{
			return _dx_vertex_shader_blob->GetBufferSize();
		}

#else



#endif

	}
}