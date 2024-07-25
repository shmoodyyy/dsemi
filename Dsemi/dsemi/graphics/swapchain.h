#ifndef DSEMI_GRAPHICS_SWAPCHAIN
#define DSEMI_GRAPHICS_SWAPCHAIN
#include <dsemi/graphics/api_include.h>
#include <dsemi/graphics/framebuffer.h>
#include <dsemi/graphics/resources/texture2d.h>

namespace dsemi
{
    class Window;
}

namespace dsemi::graphics
{
    class SwapChain
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
        auto getFrontBuffer() -> ComPtr<ID3D11Texture2D> { return m_frontBuffer; }
        // auto getRenderTarget() -> void;

#if defined (_WIN32)
        ComPtr<IDXGISwapChain> m_swapChain;
        ComPtr<ID3D11Texture2D> m_frontBuffer;
    private:
        void setFrontBuffer();
#endif
    private:
        // std::shared_ptr<FrameBuffer> m_frontBuffer;
    };
}

#endif
