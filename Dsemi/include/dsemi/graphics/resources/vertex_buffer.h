#ifndef GRAPHICS_VERTEXBUFFER_H
#define GRAPHICS_VERTEXBUFFER_H

#include "Dsemi/graphics/api_include.h"
#include "dsemi/graphics/resources/resource.h"

namespace dsemi {
	namespace graphics {
		class device;

		struct vertex_buffer_desc 
		{
			void* data_pointer = nullptr;
			uint32_t data_size = 0u;
			uint32_t data_stride = 0u;
		};

		class vertex_layout
		{
		public:
			struct element
			{

			};

		private:

		};

		class vertex_view
		{

		};

		class vertex_buffer : public iresource
		{
		public:
			vertex_buffer(device* device);
			vertex_buffer(device* device);

			inline uint32_t& get_stride()
			{
				return _stride;
			}

			void bind() const {};

		private:
			uint32_t _stride;
			const device* _device;

		private:
			ComPtr<ID3D11Buffer>      _dx_buffer;
		};

	}
}

#endif