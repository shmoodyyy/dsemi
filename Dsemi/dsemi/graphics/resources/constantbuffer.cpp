#include "constantbuffer.h"

auto dsemi::graphics::ConstantBuffer::data() -> void*
{
    return m_data;
}

void dsemi::graphics::ConstantBuffer::setBindType(BindType bindType)
{
    m_bindType = bindType;
}

auto dsemi::graphics::ConstantBuffer::getBindType() const -> BindType
{
    return m_bindType;
}
