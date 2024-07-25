#ifndef DSEMI_GRAPHICS_RESOURCES_TEXTURE2D
#define DSEMI_GRAPHICS_RESOURCES_TEXTURE2D
#include <dsemi/graphics/api_include.h>

namespace dsemi::graphics
{
    class Texture2D
    {
    public:
        Texture2D(unsigned width, unsigned height);
#ifdef _WIN32
        Texture2D(ID3D11Texture2D* dxTexture);
#endif

        ~Texture2D();

    private:
#ifdef _WIN32
        ComPtr<ID3D11Texture2D> m_texture2d;
#endif
    };
}

#endif
