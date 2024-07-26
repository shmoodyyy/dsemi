#ifndef DSEMI_GRAPHICS_SWAPCHAIN
#define DSEMI_GRAPHICS_SWAPCHAIN
#include <dsemi/graphics/api_include.h>
#include <dsemi/graphics/framebuffer.h>
#include <dsemi/graphics/resources/texture2d.h>
#include <dsemi/graphics/rendertarget.h>

namespace dsemi
{
    class Window;
}

namespace dsemi::graphics
{
    // this doesnt feel like a good approach
    // TODO: model the swapchain the way it actually is
    // using a collection of underlying framebuffers
    // still need to figure out how to properly abstract
    // that part with DX11 already doing it on its own
    class SwapChain : public RenderTarget
    {
    public:
        enum ErrNo
        {
            ok = 0,
            badMemory,
        };
        SwapChain(Window& wnd);
        SwapChain(SwapChain&) = delete;
        ~SwapChain();

        auto resize(unsigned width, unsigned height) -> ErrNo;
        auto present() -> ErrNo;
        // auto getRenderTarget() -> void;

#if defined (_WIN32)
        ComPtr<IDXGISwapChain> m_swapChain;
    private:
        void setFrameBufferTexture();
        void setRenderTargetView();
#endif
    private:
        // std::shared_ptr<FrameBuffer> m_frontBuffer;
    };
}

#endif
