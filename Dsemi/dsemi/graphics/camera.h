#ifndef DSEMI_GRAPHICS_CAMERA
#define DSEMI_GRAPHICS_CAMERA
#include <dsemi/graphics/resources/constantbuffer.h>
#include <dsemi/graphics/bindable.h>
#include <dsemi/math/Vector3f.h>
#include <memory>

namespace dsemi::graphics
{
    class Camera2D
    {
    public:
        Camera2D();
        ~Camera2D();

        void setZoom(float zoom);
        void setViewportSize(unsigned int width, unsigned int height);
        void setProjectionMatrix(float* matrix);
        void setWorldTransform(Vector3f matrix);
        void bind();

    private:
        float m_zoom;
        unsigned int m_viewportWidth;
        unsigned int m_viewportHeight;
        std::shared_ptr<ConstantBuffer> m_projectionBuffer;
        std::shared_ptr<ConstantBuffer> m_viewTransformBuffer;
    };

    class Camera3D
    {
    public:
        Camera3D();
        ~Camera3D();

        void setProjectionMatrix(float* matrix);
        void setWorldTransform(float* matrix);

    private:
        std::shared_ptr<ConstantBuffer> m_projectionBuffer;
        std::shared_ptr<ConstantBuffer> m_viewTransformBuffer;
    };
}

#endif
