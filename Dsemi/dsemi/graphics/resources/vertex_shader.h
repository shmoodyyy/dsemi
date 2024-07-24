#ifndef GRAPHICS_VERTEX_SHADER_H
#define GRAPHICS_VERTEX_SHADER_H

#include "Dsemi/graphics/api_include.h"
#include "Dsemi/graphics/resources/input_layout.h"

namespace dsemi {
	namespace graphics {

		struct vertex_shader_desc {
			const wchar_t* path = nullptr;
			//input_element* layout_elements = nullptr;
			size_t         layout_size = 1u;
		};

		class vertex_shader {
			friend class Device;
		public:
			vertex_shader()
				: _dx_vertex_shader(nullptr), _dx_vertex_shader_blob(nullptr)
			{}

			void* byte_code();
			size_t byte_code_size();
			//inline const input_layout* get_input_layout() {
			//	return &_input_layout;
			//}

		private:
			//input_layout _input_layout;

		private:
			ComPtr<ID3D11VertexShader> _dx_vertex_shader;
			ComPtr<ID3DBlob>           _dx_vertex_shader_blob;
		};
	}
}


#endif