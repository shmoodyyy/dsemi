#ifndef DSEMI_GRAPHICS_SWAPCHAIN
#define DSEMI_GRAPHICS_SWAPCHAIN
#include "dsemi/graphics/api_include.h"
#include <memory>

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
#if defined (_WIN32)
        ComPtr<IDXGISwapChain> m_swapChain;
#endif
    };
}

#endif
