#ifndef DSEMI_GRAPHICS_RENDERTARGETVIEW
#define DSEMI_GRAPHICS_RENDERTARGETVIEW
#include <dsemi/graphics/api_include.h>

namespace dsemi::graphics
{
    class RenderTarget;
    class RenderTargetView
    {
    public:
        RenderTargetView(RenderTarget* renderTarget);
        ~RenderTargetView();

#ifdef _WIN32
        ComPtr<ID3D11RenderTargetView> m_renderTargetView;
#endif

    private:
    };
}

#endif
