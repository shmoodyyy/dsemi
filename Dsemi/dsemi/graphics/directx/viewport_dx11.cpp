#include <dsemi/graphics/viewport.h>
#include <dsemi/graphics/device.h>

dsemi::graphics::Viewport::Viewport(Vector2f pos, Vector2f size, float maxDepth, float minDepth)
{
    memset(&m_d3d11Viewport, 0, sizeof(m_d3d11Viewport));
    setPosition(pos);
    setSize(size);
    m_d3d11Viewport.MaxDepth = maxDepth;
    m_d3d11Viewport.MinDepth = minDepth;
}

dsemi::graphics::Viewport::Viewport()
{
    memset(&m_d3d11Viewport, 0, sizeof(m_d3d11Viewport));
    setPosition({0, 0});
    setSize({0, 0});
    m_d3d11Viewport.MaxDepth = 1.0f;
    m_d3d11Viewport.MinDepth = 0.0f;
}

dsemi::graphics::Viewport::~Viewport()
{
}

auto dsemi::graphics::Viewport::getSize() const -> Vector2f
{
    return Vector2f(m_d3d11Viewport.Width, m_d3d11Viewport.Height);
}

auto dsemi::graphics::Viewport::getWidth() const -> float
{
    return m_d3d11Viewport.Width;
}

auto dsemi::graphics::Viewport::getHeight() const -> float
{
    return m_d3d11Viewport.Height;
}

auto dsemi::graphics::Viewport::getPosition() const -> Vector2f
{
    return Vector2f(m_d3d11Viewport.TopLeftX, m_d3d11Viewport.TopLeftY);
}

auto dsemi::graphics::Viewport::getX() const -> float
{
    return m_d3d11Viewport.TopLeftX;
}

auto dsemi::graphics::Viewport::getY() const -> float 
{
    return m_d3d11Viewport.TopLeftY;
}

void dsemi::graphics::Viewport::setSize(Vector2f val)
{
    setSize(val.x, val.y);
}

void dsemi::graphics::Viewport::setSize(float x, float y)
{
    setWidth(x);
    setHeight(y);
}

void dsemi::graphics::Viewport::setWidth(float val)
{
    m_d3d11Viewport.Width = val;
}

void dsemi::graphics::Viewport::setHeight(float val)
{
    m_d3d11Viewport.Height = val;
}

void dsemi::graphics::Viewport::setPosition(Vector2f val)
{
    setPosition(val.x, val.y);
}

void dsemi::graphics::Viewport::setPosition(float x, float y)
{
    setX(x);
    setY(y);
}

void dsemi::graphics::Viewport::setX(float val)
{
    m_d3d11Viewport.TopLeftX = val;
}

void dsemi::graphics::Viewport::setY(float val)
{
    m_d3d11Viewport.TopLeftY = val;
}

void dsemi::graphics::Viewport::bind()
{
    Device::get().get_context()->RSSetViewports(1u, &m_d3d11Viewport);
}
