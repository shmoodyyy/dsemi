#include <dsemi/graphics/viewport.h>
#include <dsemi/graphics/device.h>

dsemi::graphics::Viewport::Viewport(Vector2f pos, Vector2f size, float maxDepth, float minDepth)
    : m_position(pos)
    , m_size(size)
    , m_depthMax(maxDepth)
    , m_depthMin(minDepth)
{
    memset(&m_d3d11Viewport, 0, sizeof(m_d3d11Viewport));
    m_d3d11Viewport.TopLeftX = m_position.x;
    m_d3d11Viewport.TopLeftY = m_position.y;
    m_d3d11Viewport.Width    = m_size.x;
    m_d3d11Viewport.Height   = m_size.y;
    m_d3d11Viewport.MinDepth = m_depthMin;
    m_d3d11Viewport.MaxDepth = m_depthMax;
}

dsemi::graphics::Viewport::~Viewport()
{
}

void dsemi::graphics::Viewport::bind()
{
    Device::get().get_context()->RSSetViewports(1u, &m_d3d11Viewport);
}
