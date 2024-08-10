#ifndef GRAPHICS_VERTEX_SHADER_H
#define GRAPHICS_VERTEX_SHADER_H
#include <dsemi/graphics/api_include.h>
#include <dsemi/graphics/bindable.h>
#include <dsemi/graphics/resources/vertexlayout.h>

namespace dsemi::graphics
{
    class VertexShader : public Bindable
    {
    public:
        VertexShader(std::string_view name, std::shared_ptr<VertexLayout> layout);

        virtual void bind() override;

    private:
        std::shared_ptr<VertexLayout> m_layout;

#ifdef _WIN32
        ComPtr<ID3D11VertexShader> m_d3d11VertexShader;
        ComPtr<ID3DBlob> m_d3d11Blob;
        ComPtr<ID3D11InputLayout> m_d3d11InputLayout;
#endif
    };
}


#endif
