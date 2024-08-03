#include "dsemi/graphics/resources/vertex_buffer.h"
#include "dsemi/graphics/device.h"
#include "dsemi/util/logger.h"

// ShaderDataType TODO: move to own files
auto dsemi::graphics::ShaderDataType_size(ShaderDataType type) -> const size_t
{
    switch (type)
    {
    case ShaderDataType::FLOAT:     return 4u;
    case ShaderDataType::FLOAT2:    return 4u * 2u;
    case ShaderDataType::FLOAT3:    return 4u * 3u;
    case ShaderDataType::FLOAT4:    return 4u * 4u;
    case ShaderDataType::SINT:      return 4u;
    case ShaderDataType::SINT2:     return 4u * 2u;
    case ShaderDataType::SINT3:     return 4u * 3u;
    case ShaderDataType::SINT4:     return 4u * 4u;
    case ShaderDataType::UINT:      return 4u;
    case ShaderDataType::UINT2:     return 4u * 2u;
    case ShaderDataType::UINT3:     return 4u * 3u;
    case ShaderDataType::UINT4:     return 4u * 4u;
    case ShaderDataType::INVALID:   break;
    }
    ASSERT(false, "Passed an invalid shader_data_type.");
    return 0;
}

auto dsemi::graphics::ShaderDataType_toDXGI(ShaderDataType type) -> const DXGI_FORMAT
{  
    switch (type) {
    case ShaderDataType::FLOAT:     return DXGI_FORMAT_R32_FLOAT;
    case ShaderDataType::FLOAT2:    return DXGI_FORMAT_R32G32_FLOAT;
    case ShaderDataType::FLOAT3:    return DXGI_FORMAT_R32G32B32_FLOAT;
    case ShaderDataType::FLOAT4:    return DXGI_FORMAT_R32G32B32A32_FLOAT;
    case ShaderDataType::SINT:      return DXGI_FORMAT_R32_SINT;
    case ShaderDataType::SINT2:     return DXGI_FORMAT_R32G32_SINT;
    case ShaderDataType::SINT3:     return DXGI_FORMAT_R32G32B32_SINT;
    case ShaderDataType::SINT4:     return DXGI_FORMAT_R32G32B32A32_SINT;
    case ShaderDataType::UINT:      return DXGI_FORMAT_R32_UINT;
    case ShaderDataType::UINT2:     return DXGI_FORMAT_R32G32_UINT;
    case ShaderDataType::UINT3:     return DXGI_FORMAT_R32G32B32_UINT;
    case ShaderDataType::UINT4:     return DXGI_FORMAT_R32G32B32A32_UINT;
    case ShaderDataType::INVALID:   break;
    }
    ASSERT(false, "Passed an invalid shader_data_type.");
    return DXGI_FORMAT_UNKNOWN;
}  

// VertexLayout
dsemi::graphics::VertexLayout::VertexLayout()
    : m_stride(0u)
{
}

auto dsemi::graphics::VertexLayout::append(std::string_view semantic, ShaderDataType type) -> self&
{
    if constexpr (1)
        for (auto& element : m_elements)
            ASSERT(element.m_semantic != semantic, "Attempted to append a duplicate semantic into a vertex layout, semantic indices aren't supported yet.");
    m_elements.emplace_back(semantic.data(), type);
    Element& elem = m_elements.back();
    elem.m_offset = m_stride;
    m_stride += elem.m_size;
    return *this;
}

auto dsemi::graphics::VertexLayout::stride() const -> size_t
{
    return m_stride;
}

auto dsemi::graphics::VertexLayout::size() const -> size_t
{
    return m_elements.size();
}

auto dsemi::graphics::VertexLayout::d3dLayout() const -> std::vector<D3D11_INPUT_ELEMENT_DESC>
{
    std::vector<D3D11_INPUT_ELEMENT_DESC> out(m_elements.size());
    for (auto& element : m_elements)
        out.push_back(element.d3dElement());
    return out;
}

void dsemi::graphics::VertexLayout::bind()
{
}

dsemi::graphics::VertexLayout::VertexLayout(std::initializer_list<Element> elements)
{
    for (auto& element : elements)
        append(element.m_semantic, element.m_type);
}

auto dsemi::graphics::VertexLayout::get(size_t i) -> const VertexLayout::Element&
{
    ASSERT(i > 0 && i < m_elements.size(), "Index out of range");
    return m_elements[i];
}

auto dsemi::graphics::VertexLayout::get(std::string_view semantic) -> const VertexLayout::Element&
{
    static Element defaultElement;
    for (auto& element : m_elements)
        if (element.m_semantic == semantic)
            return element;
    return defaultElement;
}

auto dsemi::graphics::VertexLayout::begin() -> std::vector<Element>::iterator
{
    return m_elements.begin();
}

auto dsemi::graphics::VertexLayout::end() -> std::vector<Element>::iterator
{
    return m_elements.end();
}

// VertexLayout::Element
dsemi::graphics::VertexLayout::Element::Element()
    : m_semantic("")
    , m_type(ShaderDataType::INVALID)
    , m_size(0u)
    , m_offset(0u)
{
}

dsemi::graphics::VertexLayout::Element::Element(std::string semantic, ShaderDataType type)
    : m_semantic(std::move(semantic))
    , m_type(type)
    , m_size(ShaderDataType_size(type))
    , m_offset(0u)
{
}

auto dsemi::graphics::VertexLayout::Element::semantic() const -> std::string_view
{
    return m_semantic;
}

auto dsemi::graphics::VertexLayout::Element::type() const -> ShaderDataType
{
    return m_type;
}

auto dsemi::graphics::VertexLayout::Element::size() const -> size_t
{
    return m_size;
}

auto dsemi::graphics::VertexLayout::Element::offset() const -> size_t
{
    return m_offset;
}

auto dsemi::graphics::VertexLayout::Element::d3dElement() const -> D3D11_INPUT_ELEMENT_DESC
{
    // TODO: add support for same semantics with
    // different indices
    D3D11_INPUT_ELEMENT_DESC out;
    memset(&out, 0, sizeof(out));
    out.SemanticName             = m_semantic.c_str();
    out.SemanticIndex            = 0u;
    out.Format                   = ShaderDataType_toDXGI(m_type);
    out.InputSlot                = 0u;
    out.AlignedByteOffset        = D3D11_APPEND_ALIGNED_ELEMENT; // if something breaks with vertices this might be the culprit of making the first element be aligned to nothing
    out.InputSlotClass           = D3D11_INPUT_PER_VERTEX_DATA;
    out.InstanceDataStepRate     = 0u;
    return out;
}

// ------------------------------------------------------
// vertex_view implementation
//
dsemi::graphics::VertexView::VertexView(char* starting_byte, VertexLayout& layout)
    :m_layout(layout)
{
    size_t offset = 0u;
    m_elements.resize(layout.stride());
    for (auto element : m_layout)
    {
        m_elements.push_back(starting_byte + offset);
        offset += element.size();
    }
}

auto dsemi::graphics::VertexView::get(std::string_view semantic, void* pointer_to_element) -> ShaderDataType 
{
    size_t index = 0u;
    for (auto element : m_layout)
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
    if (index < 0 || index >= m_layout.size())
    {
        pointer_to_element = nullptr;
        return ShaderDataType::INVALID;
    }

    pointer_to_element = m_elements[index];
    return (m_layout.begin() + index)->type();
}

// ------------------------------------------------------
// VertexArray implementation
//
dsemi::graphics::VertexArray::VertexArray(VertexLayout layout, size_t size /*= 0u*/)
    : m_layout(layout)
{

}

// ------------------------------------------------------
// vertex_buffer implementation
//
dsemi::graphics::VertexBuffer::VertexBuffer(VertexArray& vertices, uint32_t offset /*= 0u*/) 
    : Bindable()
    , m_stride(vertices.stride())
    , m_offset(0u)
    , m_size(vertices.size())
    , m_layout(vertices.layout())
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
    GFX_THROW_FAILED(Device::get().get_dx_device()->CreateBuffer(
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
    Device::get().get_context()->IASetVertexBuffers(0u, 1u, &m_d3d11Buffer, &m_stride, &m_offset);
    GFX_LOG_DEBUG(L"bound graphics::vertex_buffer to pipeline");
}
