#include "camera.h"

dsemi::graphics::Camera2D::Camera2D()
{
}

dsemi::graphics::Camera2D::~Camera2D()
{
}

void dsemi::graphics::Camera2D::setZoom(float zoom)
{
    m_zoom = zoom;
}

void dsemi::graphics::Camera2D::setViewportSize(unsigned int width, unsigned int height)
{
    m_viewportWidth = width;
    m_viewportHeight = height;
}

void dsemi::graphics::Camera2D::setProjectionMatrix(float* matrix)
{
    m_projectionBuffer = std::make_shared<ConstantBuffer>(
        0u,
        matrix,
        16 * sizeof(float),
        16 * sizeof(float),
        BindType::cb_bindVertexShader
    );
}

void dsemi::graphics::Camera2D::setWorldTransform(Vector3f matrix)
{
    m_viewTransformBuffer = std::make_shared<ConstantBuffer>(
        1u,
        matrix,
        16 * sizeof(float),
        16 * sizeof(float),
        BindType::cb_bindVertexShader
    );
}

void dsemi::graphics::Camera2D::bind()
{
    m_projectionBuffer->bind();
    m_viewTransformBuffer->bind();
}
