#ifndef HEADER_DSEMI_GRAPHICS_VIEWPORT
#define HEADER_DSEMI_GRAPHICS_VIEWPORT
#include <dsemi/graphics/api_include.h>
#include <dsemi/graphics/bindable.h>
#include "dsemi/math/vector2.h"

namespace dsemi::graphics 
{
    class Viewport : public Bindable
    {
    public:
        Viewport();
        Viewport(Vector2f pos, Vector2f size, float maxDepth = 1.0f, float minDepth = 0.0f);
        ~Viewport();

        auto getSize() const -> Vector2f;
        auto getWidth() const -> float;
        auto getHeight() const -> float;
        auto getPosition() const -> Vector2f;
        auto getX() const -> float;
        auto getY() const -> float;

        void setSize(Vector2f val);
        void setSize(float x, float y);
        void setWidth(float val);
        void setHeight(float val);
        void setPosition(Vector2f val);
        void setPosition(float x, float y);
        void setX(float val);
        void setY(float val);

        virtual void bind() override;

    private:
//        Vector2f m_position;
//        Vector2f m_size;
//        float m_depthMax;
//        float m_depthMin;
#if defined(_WIN32)
        D3D11_VIEWPORT m_d3d11Viewport;
#endif
    };
}

#endif
