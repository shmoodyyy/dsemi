#ifndef GRAPHICS_FRAGMENT_SHADER_H
#define GRAPHICS_FRAGMENT_SHADER_H
#include <Dsemi/graphics/api_include.h>
#include <dsemi/graphics/bindable.h>

namespace dsemi::graphics 
{
    class FragmentShader : public Bindable
    {
    public:
        FragmentShader();

        virtual void bind() override;

    private:
#ifdef _WIN32
        ComPtr<ID3D11PixelShader> m_d3d11PixelShader;
        ComPtr<ID3DBlob> m_d3d11Blob;
#endif
    };
}

#endif
