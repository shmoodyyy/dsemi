#include "dsemi/graphics/Viewport.h"

dsemi::graphics::Viewport::Viewport(Vector2f pos, Vector2f size)
    : m_position(pos)
    , m_size(size)
{
}

dsemi::graphics::Viewport::~Viewport()
{
}

auto dsemi::graphics::Viewport::getSize() const -> Vector2f
{
    return m_size;
}

auto dsemi::graphics::Viewport::getWidth() const -> float
{
    return m_size.x;
}

auto dsemi::graphics::Viewport::getHeight() const -> float
{
    return m_size.y;
}

auto dsemi::graphics::Viewport::getPosition() const -> Vector2f
{
    return m_position;
}

auto dsemi::graphics::Viewport::getX() const -> float
{
    return m_position.x;
}

auto dsemi::graphics::Viewport::getY() const -> float 
{ 
    return m_position.y; 
}
