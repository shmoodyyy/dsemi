#ifndef GRAPHICS_RENDER_TARGET_H
#define GRAPHICS_RENDER_TARGET_H
#include <dsemi/graphics/api_include.h>
#include <dsemi/graphics/color.h>
#include <dsemi/graphics/bindable.h>

namespace dsemi::graphics
{
    class RenderTarget
    {
    public:
        RenderTarget();
        ~RenderTarget() = default;

        void clear();
        void set();
        colorf m_clearColor;
#if defined (_WIN32)
    protected:
        ComPtr<ID3D11Texture2D>        m_renderTexture;
        ComPtr<ID3D11RenderTargetView> m_renderTargetView;
#endif
    };
}

#endif
