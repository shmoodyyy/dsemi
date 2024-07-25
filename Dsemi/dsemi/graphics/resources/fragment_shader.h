#ifndef GRAPHICS_FRAGMENT_SHADER_H
#define GRAPHICS_FRAGMENT_SHADER_H
#include "Dsemi/graphics/api_include.h"

namespace dsemi {
	namespace graphics {

		struct fragment_shader_desc {
			const wchar_t* path = nullptr;
		};

		class fragment_shader {
			friend class Device;
		public:
			fragment_shader()
				: _dx_pixel_shader(nullptr), _dx_pixel_shader_blob(nullptr)
			{}

			void* byte_code();
			size_t byte_code_size();

		private:
			ComPtr<ID3D11PixelShader> _dx_pixel_shader;
			ComPtr<ID3DBlob>          _dx_pixel_shader_blob;
		};
	}
}

#endif
