#pragma once
#include "dsemi/graphics/api_include.h"
#include "dsemi/graphics/resources/resource.h"
#include "dsemi/graphics/bindable.h"
#include "dsemi/util/assert.h"
#include <vector>
#include <utility>

namespace dsemi
{
	namespace graphics
	{
		class device;

		// TODO: 
		// consider possible issues that could come up due to byte padding
		// to keep 4-byte alignment of allocated memory
		//

		// todo: move this to a header file unifying vertex/index/instance buffers
		enum class shader_data_type
		{
			INVALID = 0,
			FLOAT, FLOAT2, FLOAT3, FLOAT4,
			SINT, SINT2, SINT3, SINT4,
			UINT, UINT2, UINT3, UINT4,
		}; // enum class shader_data_type

		static size_t shader_data_type_size(shader_data_type type)
		{
			switch (type)
			{
			case shader_data_type::FLOAT:	return 4u;
			case shader_data_type::FLOAT2:	return 4u * 2u;
			case shader_data_type::FLOAT3:	return 4u * 3u;
			case shader_data_type::FLOAT4:	return 4u * 4u;
			case shader_data_type::SINT:	return 4u;
			case shader_data_type::SINT2:	return 4u * 2u;
			case shader_data_type::SINT3:	return 4u * 3u;
			case shader_data_type::SINT4:	return 4u * 4u;
			case shader_data_type::INVALID:	return 0u;
			}
			ASSERT(false, "Passed an invalid shader_data_type.");
			return 0;
		}

		static DXGI_FORMAT shader_data_type_to_dx(shader_data_type type)
		{
			switch (type)
			{
			case shader_data_type::FLOAT:	return DXGI_FORMAT_R32_FLOAT;
			case shader_data_type::FLOAT2:	return DXGI_FORMAT_R32G32_FLOAT;
			case shader_data_type::FLOAT3:	return DXGI_FORMAT_R32G32B32_FLOAT;
			case shader_data_type::FLOAT4:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case shader_data_type::SINT:	return DXGI_FORMAT_R32_SINT;
			case shader_data_type::SINT2:	return DXGI_FORMAT_R32G32_SINT;
			case shader_data_type::SINT3:	return DXGI_FORMAT_R32G32B32_SINT;
			case shader_data_type::SINT4:	return DXGI_FORMAT_R32G32B32A32_SINT;
			}
			ASSERT(false, "Passed an invalid shader_data_type.");
			return DXGI_FORMAT_UNKNOWN;
		}

		class vertex_layout
		{
		public:

			class element
			{
				friend class vertex_layout;
			public:
				// idea: look into how std::string and std::string_view works
				// for taking ownership of strings provided into function parameters
				// or maybe make a custom string class to handle transfering
				// of string content ownership as hardcoded const char* parameter
				// values might end up floating around in memory with a lifetime
				// equivalent to the application lifetime
				element() = default;
				element(std::string semantic, shader_data_type type);
				const std::string& get_semantic()    const { return _semantic; }
				shader_data_type         get_type()        const { return _type; }
				// returns size_t in bytes
				size_t                   get_size()        const { return _size; }
				size_t                   get_offset()      const { return _offset; }
				D3D11_INPUT_ELEMENT_DESC get_d3d_element() const;

			private:
				std::string      _semantic;
				shader_data_type _type;
				size_t           _size;
				size_t           _offset;
			}; // class element

			vertex_layout();
			vertex_layout(const std::initializer_list<element>& elements);

			vertex_layout& append(const std::string& semantic, shader_data_type type);

			// vertex size in bytes
			size_t                                get_stride() const { return _stride; }
			size_t                                get_element_count() const { return _elements.size(); }
			std::vector<D3D11_INPUT_ELEMENT_DESC> get_d3d_layout() const;

			const element& get_by_index(size_t i);
			const element& get_by_semantic(const std::string& semantic);

			std::vector<element>::iterator begin() { return _elements.begin(); }
			std::vector<element>::iterator end()   { return _elements.end(); }

		private:
			static element       _invalid_element;
			std::vector<element> _elements;
			size_t               _stride;
		};



		// todo: add some template (meta)programming into getters and types to allow for a cleaner interface
		// of accessing vertex data without requiring manual type checking and casting
		// the data buffer to the correct type
		//
		class vertex_view
		{
			friend class vertex_array;
		protected:
			vertex_view(char* starting_byte, vertex_layout& layout);

		public:
			shader_data_type get_by_semantic(const std::string& semantic, void* pointer_to_element);
			shader_data_type get_by_index(size_t index, void* pointer_to_element);

			template<typename T>
			void set_by_index(size_t index, T&& value)
			{
				ASSERT(index > 0 && index < _layout.get_element_count(), "Index out of range");
				//ASSERT(_layout.get_by_index(index).get_size() == sizeof(T), "Vertex parameter input memory size mismatch. \n [SEMANTIC: " + _layout.get_by_index(index).get_semantic() + "]");

				*(T*)_element_pointers[_layout.get_by_index(index).get_offset()] = value;
			}
			template<typename T>
			void set_by_index(size_t index, std::initializer_list<T>&& values)
			{
				ASSERT(index > 0 && index < _layout.get_element_count(), "Index out of range");

				ASSERT(_layout.get_by_index(index).get_size() == sizeof(T) * values.size(), "Vertex parameter input memory size mismatch. \n [SEMANTIC: " + _layout.get_by_index(index).get_semantic() + "]");
				*(T**)_element_pointers[_layout.get_by_index(index).get_offset()] = values.begin();
			}

		private:
			// parameter packing
			template<typename param_first, typename ...param_rest>
			void set_by_index(size_t index, param_first&& first, param_rest&&... rest)
			{
				set_by_index(index, std::forward<param_first>(first));
				set_by_index(index + 1, std::forward<param_rest>(rest)...);
			}
			/*template<typename T>
			void set_by_index(size_t index, T&& value)
			{
			}*/

		private:
			std::vector<char*> _element_pointers;
			vertex_layout& _layout;

		}; // class vertex



		class vertex_array
		{
		public:
			vertex_array(vertex_layout layout, size_t size = 0u);

			inline char*                get_data()                { return _bytes.data(); }
			inline size_t               get_vertex_count()  const { return _bytes.size() / _layout.get_stride(); }
			inline size_t               get_vertex_stride() const { return _layout.get_stride(); };
			inline size_t               get_byte_width()    const { return _bytes.size(); };
			inline const vertex_layout& get_layout()        const { return _layout; }

			template<typename ...param_types>
			vertex_array& emplace_back(param_types&&... params)
			{
				// TODO: assert that size and count of elements match to the vertex layout
				//ASSERT(sizeof...(param_types) == _layout.get_element_count(), "Vertex parameter count doesn't match the specified vertex_array layout element count.");
				_emplace_back_set_data(std::forward<param_types>(params)...);
				return *this;
			}

		private:
			template<typename T, typename... types>
			void _emplace_back_set_data(T param_first, types... param_rest)
			{
				_emplace_back_set_data(param_first);
				_emplace_back_set_data(param_rest...);
			}
			template<typename T>
			void _emplace_back_set_data(T param)
			{
				_bytes.resize(_bytes.size() + sizeof(T));
				auto tail = (_bytes.end() - sizeof(T));
				auto dst = &*tail;
				auto src = reinterpret_cast<char*>(&param);
				memcpy(dst, src, sizeof(T));
				/*_bytes.resize(_bytes.size() + sizeof(T));
				auto tail = _bytes.end() - sizeof(T);*/
				//*(reinterpret_cast<T*>(&(*tail))) = param;
			}

			std::vector<char> _bytes; // TODO: storing vertices in cpu memory is dumb outside of a dynamic vertex buffer
			vertex_layout     _layout;
		};

		class vertex_buffer : public bindable
		{
		public:
			vertex_buffer(device* device, vertex_array& vertices, uint32_t offset = 0u);
			~vertex_buffer();

			inline size_t get_stride() { return _stride; }
			inline size_t get_count() { return _count; }
			//inline size_t get_byte_width()

			inline virtual void bind() const override;
			//vertex_view operator[](size_t i);

		protected:

		private:
			uint32_t		_stride;
			uint32_t		_offset;
			uint32_t		_count;
			vertex_layout	_layout;
			ID3D11Buffer*	_dx_buffer;
		};

		// TODO: dynamic vertex buffer class
		//
	}
}
