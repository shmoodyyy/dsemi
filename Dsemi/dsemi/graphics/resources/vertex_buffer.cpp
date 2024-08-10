#include <dsemi/graphics/resources/vertex_buffer.h>
#include <dsemi/graphics/device.h>
#include <dsemi/util/logger.h>

// ------------------------------------------------------
// vertex_view implementation
//
dsemi::graphics::VertexView::VertexView(char* starting_byte, std::shared_ptr<VertexLayout> layout)
    : m_layout(layout)
{
    size_t offset = 0u;
        m_elements.resize(m_layout->stride());
        for (auto element : *m_layout)
        {
            m_elements.push_back(starting_byte + offset);
            offset += element.size();
        }
    }

    auto dsemi::graphics::VertexView::get(std::string_view semantic, void* pointer_to_element) -> ShaderDataType 
    {
        size_t index = 0u;
        for (auto element : *m_layout)
        {
            if (element.semantic() == semantic)
            {
                pointer_to_element = m_elements[index];
                return element.type();
            }
            ++index;
        }
        return ShaderDataType::INVALID;
    }

    auto dsemi::graphics::VertexView::get(size_t index, void* pointer_to_element) -> ShaderDataType 
    {
        if (index < 0 || index >= m_layout->size())
        {
            pointer_to_element = nullptr;
            return ShaderDataType::INVALID;
        }

        pointer_to_element = m_elements[index];
        return (m_layout->begin() + index)->type();
}

// ------------------------------------------------------
// vertex_buffer implementation
//
dsemi::graphics::VertexBuffer::VertexBuffer(VertexArray& vertices, uint32_t offset /*= 0u*/) 
    : Bindable()
    , m_stride(vertices.stride())
    , m_offset(0u)
    , m_size(vertices.size())
    , m_d3d11Buffer(nullptr)
{
    // =======================================================
    //        CREATE VERTEX BUFFER
    // =======================================================
    D3D11_BUFFER_DESC bd   = {};
    bd.ByteWidth           = vertices.byteWidth();
    bd.Usage               = D3D11_USAGE_DEFAULT;
    bd.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags      = 0u;
    bd.MiscFlags           = 0u;
    bd.StructureByteStride = vertices.stride();
    D3D11_SUBRESOURCE_DATA bsrd = {};
    bsrd.pSysMem = vertices.data();

    HRESULT hr;
    GFX_THROW_FAILED(Device::get().getDxDevice()->CreateBuffer(
        &bd,
        &bsrd,
        &m_d3d11Buffer
    ));
    GFX_LOG_DEBUG(L"created graphics::vertex_buffer");
}

dsemi::graphics::VertexBuffer::~VertexBuffer()
{
    if (m_d3d11Buffer) { 
        m_d3d11Buffer->Release(); 
        m_d3d11Buffer = nullptr; 
    }
    GFX_LOG_DEBUG(L"released graphics::vertex_buffer");
}

void dsemi::graphics::VertexBuffer::bind()
{
    Device::get().getContext()->IASetVertexBuffers(0u, 1u, &m_d3d11Buffer, &m_stride, &m_offset);
    GFX_LOG_DEBUG(L"bound graphics::vertex_buffer to pipeline");
}
