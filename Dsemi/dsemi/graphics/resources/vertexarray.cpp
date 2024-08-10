#include "vertexarray.h"

dsemi::graphics::VertexArray::VertexArray(std::shared_ptr<VertexLayout> layout, size_t capacity /*= 0u*/)
    : m_data(nullptr)
    , m_size(0)
    , m_capacity(capacity)
    , m_layout(layout)
{
}

dsemi::graphics::VertexArray::~VertexArray()
{
    if (m_data != nullptr)
        free(m_data);
}

auto dsemi::graphics::VertexArray::data() const -> const void* const
{
    return m_data;
}

auto dsemi::graphics::VertexArray::size() const -> const size_t
{
    return m_size;
}

// 10.08.2024: TODO, use a different term than stride because its a really bad name
//             adapted from directx to describe per vertex byte size
auto dsemi::graphics::VertexArray::stride() const -> const size_t
{
    return m_layout->stride();
}

auto dsemi::graphics::VertexArray::capacity() const -> const size_t
{
    return m_capacity;
}

auto dsemi::graphics::VertexArray::byteWidth() const -> const size_t
{
    return m_layout->stride() * m_size;
}

auto dsemi::graphics::VertexArray::layout() const -> std::shared_ptr<VertexLayout>
{
    return nullptr;
}

void dsemi::graphics::VertexArray::resize(std::size_t newCapacity)
{
    if (newCapacity <= m_capacity) [[unlikely]]
        return;
    void* old = m_data;
    m_data = malloc(newCapacity * m_layout->stride());
    m_capacity = newCapacity;
    if (old == nullptr) [[unlikely]]
        return;
    memcpy(m_data, old, m_size * m_layout->stride());
    free(old);
}
