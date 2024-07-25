#ifndef DSEMI_GRAPHICS_FRAMEBUFFER
#define DSEMI_GRAPHICS_FRAMEBUFFER
#include "dsemi/graphics/api_include.h"

namespace dsemi::graphics
{
    class FrameBuffer
    {
    public:
        FrameBuffer();
        ~FrameBuffer();

    private:
#ifdef _WIN32
        ComPtr<ID3D11Texture2D> m_buffer;
#endif
    };
}

#endif
