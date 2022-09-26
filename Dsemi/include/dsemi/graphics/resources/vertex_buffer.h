#pragma once

#include "Dsemi/graphics/api_include.h"
#include "dsemi/graphics/resources/resource.h"
#include <vector>

namespace dsemi {
	namespace graphics {
		class device;

		// TODO: 
		// consider possible issues that could (WILL) come up due to byte padding
		// to keep 4-byte alignment of allocated memory
		//

		// todo: move this to a header file unifying vertex/index/instance buffers
		enum class shader_data_type
		{
			INVALID = 0,
			FLOAT, FLOAT2, FLOAT3, FLOAT4,
		}; // enum class shader_data_type

		static size_t shader_data_type_size(shader_data_type type)
		{
			switch (type)
			{
			case shader_data_type::FLOAT:   return 4u;
			case shader_data_type::FLOAT2:  return 4u * 2u;
			case shader_data_type::FLOAT3:  return 4u * 3u;
			case shader_data_type::FLOAT4:  return 4u * 4u;
			case shader_data_type::INVALID: return 0u;
			}

			ASSERT(false, "Passed an invalid shader_data_type.");
			return 0;
		}

		static DXGI_FORMAT shader_data_type_to_dx(shader_data_type type)
		{
			switch (type)
			{
			case shader_data_type::FLOAT:   return DXGI_FORMAT_R32_FLOAT;
			case shader_data_type::FLOAT2:  return DXGI_FORMAT_R32G32_FLOAT;
			case shader_data_type::FLOAT3:  return DXGI_FORMAT_R32G32B32_FLOAT;
			case shader_data_type::FLOAT4:  return DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
			ASSERT(false, "Passed an invalid shader_data_type.")
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
				element(const std::string& semantic, shader_data_type type);
				const std::string&       get_semantic()    const noexcept { return _semantic;}
				shader_data_type         get_type()        const noexcept { return _type; }
				// returns size_t in bytes
				size_t                   get_size()        const noexcept { return _size; }
				size_t                   get_offset()      const noexcept { return _offset; }
				D3D11_INPUT_ELEMENT_DESC get_d3d_element() const noexcept;

			private:
				std::string      _semantic;
				shader_data_type _type;
				size_t           _size;
				size_t           _offset;
			}; // class element

			vertex_layout();
			vertex_layout(const std::initializer_list<element>& elements);

			vertex_layout&                        append(const std::string& semantic, shader_data_type type);
			
			// vertex size in bytes
			size_t                                get_stride() const noexcept { return _stride; }
			size_t                                get_element_count() const noexcept { return _elements.size(); }
			std::vector<D3D11_INPUT_ELEMENT_DESC> get_d3d_layout() const noexcept;

			const element& get_by_index(size_t i);
			const element& get_by_semantic(const std::string& semantic);

			std::vector<element>::iterator begin() noexcept { return _elements.begin(); }
			std::vector<element>::iterator end()   noexcept { return _elements.end(); }

		private:
			static element       _invalid_element;
			std::vector<element> _elements;
			size_t               _stride;
		}; // class vertex_layout



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
			// returns element_type::INVALID if semantic not found
			shader_data_type get_by_semantic(const std::string& semantic, void* pointer_to_element);
			// returns element_type::INVALID if index out of range
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

				ASSERT(_layout.get_by_index(index).get_size() == sizeof(T * values.size()), "Vertex parameter input memory size mismatch. \n [SEMANTIC: " + _layout.get_by_index(index).get_semantic() + "]");
				*(T**)_element_pointers[_layout.get_by_index(index).get_offset()] = values.begin();
			}

		private:
			// parameter packing
			template<typename param_first, typename ...param_rest>
			void set_by_index(size_t index, param_first&& first, param_rest&&... rest)
			{
				set_by_index(index, std::forward<param_first>(first));	
				set_by_index(index + 1, std::forward<param_rest>(rest...));
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

			char*                get_data()                noexcept { return _bytes.data(); }
			size_t               get_vertex_count()  const noexcept { return _bytes.size() / _layout.get_element_count(); }
			size_t               get_vertex_stride() const noexcept { return _layout.get_stride(); };
			const vertex_layout& get_layout()        const noexcept { return _layout; }

			template<typename ...param_types>
			void emplace_back(param_types&&... params)
			{
				ASSERT(sizeof...(param_types) == _layout.get_element_count(), "Vertex parameter count doesn't match the specified vertex_array layout element count.");
				_bytes.resize(_bytes.size() + _layout.get_stride());
				_emplace_back_set_data(params...);
			}

		private:
			template<typename T, typename... types>
			void _emplace_back_set_data(T&& param_first, types&&... param_rest)
			{

			}

			std::vector<char> _bytes; // todo: storing vertices in cpu memory is dumb outside of a dynamic vertex buffer
			vertex_layout     _layout;
		};



		class vertex_buffer : public iresource
		{
		public:
			vertex_buffer(device* device, vertex_array vertices);

			inline uint32_t& get_stride()
			{
				return _stride;
			}

			void bind() const {};
			//vertex_view operator[](size_t i);


		private:
			uint32_t      _stride;
			const device* _device;

		private:
			vertex_layout        _layout;
			ComPtr<ID3D11Buffer> _dx_buffer;
		}; // class vertex_buffer

		// TODO: dynamic vertex buffer class
	} // namespace graphics
} // namespace dsemi