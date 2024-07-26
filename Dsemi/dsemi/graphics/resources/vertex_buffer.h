#pragma once
#include "dsemi/graphics/api_include.h"
#include "dsemi/graphics/resources/resource.h"
#include "dsemi/graphics/bindable.h"
#include "dsemi/util/assert.h"
#include <vector>
#include <utility>

namespace dsemi::graphics
{
    // TODO: 
    // consider possible issues that could come up due to byte padding
    // to keep 4-byte alignment of allocated memory
    // 26.07.2024: my past haunts my future with bad comments

    // todo: move this to a header file unifying vertex/index/instance buffers
    enum class ShaderDataType
    {
        INVALID = 0,
        FLOAT, FLOAT2, FLOAT3, FLOAT4,
        SINT, SINT2, SINT3, SINT4,
        UINT, UINT2, UINT3, UINT4,
    };
    auto ShaderDataType_size(ShaderDataType type) -> const size_t;
    auto ShaderDataType_toDXGI(ShaderDataType type) -> const DXGI_FORMAT;

    class VertexLayout
    {
        using self = VertexLayout;
    public:
        VertexLayout();

        auto append(std::string_view semantic, ShaderDataType type) -> VertexLayout&;
        auto stride() const -> size_t;
        auto size() const -> size_t;
        auto d3dLayout() const -> std::vector<D3D11_INPUT_ELEMENT_DESC>;

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
            auto d3dElement() const -> D3D11_INPUT_ELEMENT_DESC;

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
        std::vector<Element> m_elements;
        size_t               m_stride;
    };

    // todo: add some template (meta)programming into getters and types to allow for a cleaner interface
    // of accessing vertex data without requiring manual type checking and casting
    // the data buffer to the correct type
    //
    class VertexView
    {
        friend class VertexArray;
    protected:
        VertexView(char* starting_byte, VertexLayout& layout);

    public:
        ShaderDataType get(std::string_view semantic, void* pointer_to_element);
        ShaderDataType get(size_t index, void* pointer_to_element);

        // these templates were a lot more fun at the time of writing them
        template<typename T>
        void setByIndex(size_t index, T&& value)
        {
            ASSERT(index > 0 && index < m_layout.size(), "Index out of range");
            //ASSERT(_layout.get_by_index(index).get_size() == sizeof(T), "Vertex parameter input memory size mismatch. \n [SEMANTIC: " + _layout.get_by_index(index).get_semantic() + "]");

            *(T*)m_elements[m_layout.get(index).offset()] = value;
        }

        template<typename T>
        void setByIndex(size_t index, std::initializer_list<T>&& values)
        {
            ASSERT(index > 0 && index < m_layout.size(), 
                "Index out of range");
            ASSERT(m_layout.get(index).size() == sizeof(T) * values.size(), 
                "Vertex parameter input memory size mismatch. \n [SEMANTIC: " << m_layout.get(index).semantic() << "]");
            *(T**)m_elements[m_layout.get(index).offset()] = values.begin();
        }

    private:
        // parameter packing
        template<typename param_first, typename ...param_rest>
        void setByIndex(size_t index, param_first&& first, param_rest&&... rest)
        {
            setByIndex(index, std::forward<param_first>(first));
            setByIndex(index + 1, std::forward<param_rest>(rest)...);
        }
        /*template<typename T>
        void setByIndex(size_t index, T&& value)
        {
        }*/

    private:
        std::vector<char*> m_elements;
        VertexLayout& m_layout;
    };

    class VertexArray
    {
    public:
        VertexArray(VertexLayout layout, size_t size = 0u);

        auto data() const -> const char*            { return m_bytes.data(); } // will i ever use this?
        auto size() const -> size_t                 { return m_bytes.size() / m_layout.stride(); }
        auto stride() const -> size_t               { return m_layout.stride(); };
        auto byteWidth() const -> size_t            { return m_bytes.size(); };
        auto layout() const -> const VertexLayout&  { return m_layout; }

        template<typename ...param_types>
        VertexArray& emplaceBack(param_types&&... params)
        {
            // TODO: assert that size and count of elements match to the vertex layout
            //ASSERT(sizeof...(param_types) == _layout.get_element_count(), "Vertex parameter count doesn't match the specified vertex_array layout element count.");
            emplaceBack_setData(std::forward<param_types>(params)...);
            return *this;
        }

    private:
        // what does 'setData' mean here?
        template<typename T, typename... types>
        void emplaceBack_setData(T param_first, types... param_rest)
        {
            emplaceBack_setData(param_first);
            emplaceBack_setData(param_rest...);
        }
        template<typename T>
        void emplaceBack_setData(T param)
        {
            m_bytes.resize(m_bytes.size() + sizeof(T));
            auto tail = (m_bytes.end() - sizeof(T));
            auto dst = &*tail;
            auto src = reinterpret_cast<char*>(&param);
            memcpy(dst, src, sizeof(T));
            /*_bytes.resize(_bytes.size() + sizeof(T));
            auto tail = _bytes.end() - sizeof(T);*/
            //*(reinterpret_cast<T*>(&(*tail))) = param;
        }

        std::vector<char> m_bytes; // TODO: dont do this outside of dynamic buffers
        VertexLayout m_layout;
    };

    class VertexBuffer : public Bindable
    {
    public:
        VertexBuffer(VertexArray& vertices, uint32_t offset = 0u);
        ~VertexBuffer();

        auto stride() -> size_t { return m_stride; }
        auto size() -> size_t { return m_size; }
        //inline size_t get_byte_width()

        virtual void bind() override;
        //VertexView operator[](size_t i);

    private:
        uint32_t m_stride;
        uint32_t m_offset;
        uint32_t m_size;
        VertexLayout m_layout;
        ID3D11Buffer* m_d3d11Buffer;
    };

    // TODO: dynamic vertex buffer class
        //
}
