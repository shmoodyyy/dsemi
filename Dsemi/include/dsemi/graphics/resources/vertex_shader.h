#ifndef GRAPHICS_VERTEX_SHADER_H
#define GRAPHICS_VERTEX_SHADER_H

#include "Dsemi/graphics/api_include.h"
#include "Dsemi/graphics/resources/input_layout.h"

namespace dsemi {

	struct gfx_vertex_shader_desc {
		const wchar_t*     path            = nullptr;
		gfx_input_element* layout_elements = nullptr;
		size_t             layout_size     = 1u;
	};

	class gfx_vertex_shader {
		friend class gfx_device;
	public:
		gfx_vertex_shader() 
			: _dx_vertex_shader(nullptr), _dx_vertex_shader_blob(nullptr)
		{}

		// todo: set return type based on what api we are using
		void*  byte_code();
		size_t byte_code_size();
		inline const gfx_input_layout* input_layout() {
			return &_input_layout;
		}

	private:
		gfx_input_layout _input_layout;

#ifdef GFX_USING_DX11
	private:
		ComPtr<ID3D11VertexShader> _dx_vertex_shader;
		ComPtr<ID3DBlob>           _dx_vertex_shader_blob;
#endif
	};

}


#endif