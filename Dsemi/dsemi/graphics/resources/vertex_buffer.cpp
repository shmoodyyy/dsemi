#include "dsemi/graphics/resources/vertex_buffer.h"
#include "dsemi/graphics/device.h"
#include "dsemi/util/logger.h"

namespace dsemi 
{
	namespace graphics
	{
		vertex_layout::element::element(std::string semantic, shader_data_type type)
			: _semantic(std::move(semantic))
            , _type(type)
            , _size(0u)
		{
			_size = shader_data_type_size(type);
		}

		D3D11_INPUT_ELEMENT_DESC vertex_layout::element::get_d3d_element() const
		{
			// TODO: add support for same semantics with
			// different indices
			D3D11_INPUT_ELEMENT_DESC out = {0};
			out.SemanticName             = _semantic.c_str();
			out.SemanticIndex            = 0u;
			out.Format                   = shader_data_type_to_dx(_type);
			out.InputSlot                = 0u;
			out.AlignedByteOffset        = D3D11_APPEND_ALIGNED_ELEMENT; // if something breaks with vertices this might be the culprit of making the first element be aligned to nothing
			out.InputSlotClass           = D3D11_INPUT_PER_VERTEX_DATA;
			out.InstanceDataStepRate     = 0u;
			return out;
		}

		// ------------------------------------------------------
		// vertex_layout implementation
		//
		vertex_layout::element vertex_layout::_invalid_element("", shader_data_type::INVALID);

		vertex_layout& vertex_layout::append(const std::string& semantic, shader_data_type type)
		{
#ifdef _DEBUG // debug build input validation
			for (auto& element : _elements)
			{
				ASSERT(element._semantic != semantic, "Attempted to append a duplicate semantic into a vertex layout, semantic indices aren't supported yet.");
			}
#endif
			_elements.emplace_back(semantic, type);
			element& elem = _elements.back();
			elem._offset = _stride;
			_stride += elem._size;

			return *this;
		}

		vertex_layout::vertex_layout()
			:_stride(0u)
		{
		}

		vertex_layout::vertex_layout(const std::initializer_list<element>& elements)
		{
			for (auto element : elements)
			{
				append(element._semantic, element._type);
			}
		}

		std::vector<D3D11_INPUT_ELEMENT_DESC> vertex_layout::get_d3d_layout() const
		{
			std::vector<D3D11_INPUT_ELEMENT_DESC> out;
			for (auto element : _elements)
			{
				out.push_back(element.get_d3d_element());
			}
			return out;
		}

		const vertex_layout::element& vertex_layout::get_by_index(size_t i)
		{
			ASSERT(i > 0 && i < _elements.size(), "Index out of range");
			return _elements[i];
		}

		const vertex_layout::element& vertex_layout::get_by_semantic(const std::string& semantic)
		{
			for (auto element : _elements)
			{
				if (element._semantic == semantic)
					return element;
			}
			return _invalid_element;
		}



		// ------------------------------------------------------
		// vertex_view implementation
		//
		vertex_view::vertex_view(char* starting_byte, vertex_layout& layout)
			:_layout(layout)
		{
			size_t offset = 0u;
			_element_pointers.resize(layout.get_stride());
			for (auto element : _layout)
			{
				_element_pointers.push_back(starting_byte + offset);
				offset += element.get_size();
			}
		}

		shader_data_type vertex_view::get_by_semantic(const std::string& semantic, void* pointer_to_element)
		{
			size_t index = 0u;
			for (auto element : _layout)
			{
				if (element.get_semantic() == semantic)
				{
					pointer_to_element = _element_pointers[index];
					return element.get_type();
				}
				++index;
			}
			pointer_to_element == nullptr;
			return shader_data_type::INVALID;
		}

		shader_data_type vertex_view::get_by_index(size_t index, void* pointer_to_element)
		{
			if (index < 0 || index >= _layout.get_element_count())
			{
				pointer_to_element = nullptr;
				return shader_data_type::INVALID;
			}

			pointer_to_element = _element_pointers[index];
			return (_layout.begin() + index)->get_type();
		}

		// ------------------------------------------------------
		// vertex_array implementation
		//
		vertex_array::vertex_array(vertex_layout layout, size_t size /*= 0u*/)
			: _layout(layout)
		{

		}

		// ------------------------------------------------------
		// vertex_buffer implementation
		//
		vertex_buffer::vertex_buffer(Device* device, vertex_array& vertices, uint32_t offset /*= 0u*/) : bindable(device), _layout(vertices.get_layout()), _dx_buffer(nullptr), _stride(vertices.get_vertex_stride()), _offset(0u), _count(vertices.get_vertex_count())
		{
			// =======================================================
			//		CREATE VERTEX BUFFER
			// =======================================================
			D3D11_BUFFER_DESC bd   = {};
			bd.ByteWidth           = vertices.get_byte_width();
			bd.Usage               = D3D11_USAGE_DEFAULT;
			bd.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags      = 0u;
			bd.MiscFlags           = 0u;
			bd.StructureByteStride = vertices.get_vertex_stride();
			D3D11_SUBRESOURCE_DATA bsrd = {};
			bsrd.pSysMem = vertices.get_data();

			HRESULT hr;
			GFX_THROW_FAILED(getDevice().get_dx_device()->CreateBuffer(
				&bd,
				&bsrd,
				&_dx_buffer
			));
			GFX_LOG_DEBUG(L"created graphics::vertex_buffer");
		}

		vertex_buffer::~vertex_buffer()
		{
			if (_dx_buffer) { _dx_buffer->Release(); ; _dx_buffer = nullptr; }
			GFX_LOG_DEBUG(L"released graphics::vertex_buffer");
		}

		void vertex_buffer::bind() const
		{
			getDevice().get_context()->IASetVertexBuffers(0u, 1u, &this->_dx_buffer, &this->_stride, &_offset);
			GFX_LOG_DEBUG(L"bound graphics::vertex_buffer to pipeline");
		}


	} // namespace graphics
} // namespace dsemi
