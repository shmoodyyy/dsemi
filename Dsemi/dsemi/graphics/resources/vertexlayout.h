#ifndef DSEMI_GRAPHICS_VERTEXLAYOUT
#define DSEMI_GRAPHICS_VERTEXLAYOUT
#include <dsemi/graphics/api_include.h>
#include <dsemi/graphics/bindable.h>
#include <string_view>

/*
    09.08.2024:
        currently the VertexLayout is completely mutable,
        to be decided if having the underlying API resource reconstructed
        on each mutating function call is too horrible of a design,
        but if you mutate it past when you first define it,
        then its going to invalidate the binding stage and at that point you
        **should** know whats going wrong.
 */

namespace dsemi::graphics
{
    // todo: move this to a header file unifying vertex/index/instance buffers
    enum class ShaderDataType
    {
        INVALID = 0,
        FLOAT, FLOAT2, FLOAT3, FLOAT4,
        SINT, SINT2, SINT3, SINT4,
        UINT, UINT2, UINT3, UINT4,
    };
    constexpr auto ShaderDataType_size(ShaderDataType type) -> const size_t;
    auto ShaderDataType_toDxgi(ShaderDataType type) -> const DXGI_FORMAT;

    class VertexLayout : public Bindable
    {
    public:
        VertexLayout();

        auto append(std::string_view semantic, ShaderDataType type) -> VertexLayout&;
        auto stride() const -> size_t;
        auto size() const -> size_t;

        virtual void bind() override;

        class Element
        {
            friend class VertexLayout;
        public:
            Element();
            Element(std::string semantic, ShaderDataType type);
            auto semantic() const -> std::string_view;
            auto type() const -> ShaderDataType;
            auto size() const -> size_t;
            auto offset() const -> size_t;

        private:
            std::string     m_semantic;
            ShaderDataType  m_type;
            size_t          m_size;
            size_t          m_offset;
        };
        VertexLayout(std::initializer_list<Element> elements);
        auto get(size_t i) -> const Element&;
        auto get(std::string_view semantic) -> const Element&;
        auto begin() -> std::vector<Element>::iterator;
        auto end() -> std::vector<Element>::iterator;

    private:
        void createApiResource();

        std::vector<Element> m_elements;
        size_t               m_stride;

#ifdef WIN32
        ComPtr<ID3D11InputLayout> m_d3d11InputLayout;
        std::vector<D3D11_INPUT_ELEMENT_DESC> m_d3d11Elements;
#endif
    };
}

#endif
