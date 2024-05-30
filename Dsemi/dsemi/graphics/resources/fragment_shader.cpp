#include "Dsemi/graphics/resources/fragment_shader.h"

namespace dsemi {
	namespace graphics {

		void* fragment_shader::byte_code()
		{
			return _dx_pixel_shader_blob->GetBufferPointer();
		}

		size_t fragment_shader::byte_code_size()
		{
			return _dx_pixel_shader_blob->GetBufferSize();
		}

	}
}
