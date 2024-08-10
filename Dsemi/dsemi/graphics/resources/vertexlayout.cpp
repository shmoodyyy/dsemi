#include "vertexlayout.h"
#include <dsemi/util/assert.h>

// ShaderDataType TODO: move to own files
constexpr auto dsemi::graphics::ShaderDataType_size(ShaderDataType type) -> const size_t
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

auto dsemi::graphics::ShaderDataType_toDxgi(ShaderDataType type) -> const DXGI_FORMAT
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

auto dsemi::graphics::VertexLayout::append(std::string_view semantic, ShaderDataType type) -> VertexLayout&
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

void dsemi::graphics::VertexLayout::bind()
{
    //Device::get().getContext()->IASetInputLayout(m_d3d11Elements.data());
}

dsemi::graphics::VertexLayout::VertexLayout(std::initializer_list<Element> elements)
{
    for (auto& element : elements)
        append(element.m_semantic, element.m_type);
}

auto dsemi::graphics::VertexLayout::get(size_t i) -> const VertexLayout::Element&
{
    ASSERT(i >= 0 && i < m_elements.size(), "Index out of range");
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
