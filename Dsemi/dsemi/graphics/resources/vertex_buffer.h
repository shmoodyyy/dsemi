#pragma once
#include <dsemi/graphics/bindable.h>
#include <dsemi/graphics/resources/vertexarray.h>
#include <dsemi/util/assert.h>
#include <vector>
#include <utility>

namespace dsemi::graphics
{
    // todo: add some template (meta)programming into getters and types to allow for a cleaner interface
    // of accessing vertex data without requiring manual type checking and casting
    // the data buffer to the correct type
    //
    // 09.08.2024: every old comment may as well be fluffed with a big "what?"
    class VertexView
    {
        // 03.08.2024: what is this class for again?
        friend class VertexArray;
    protected:
        VertexView(char* starting_byte, std::shared_ptr<VertexLayout> layout);

    public:
        ShaderDataType get(std::string_view semantic, void* pointer_to_element);
        ShaderDataType get(size_t index, void* pointer_to_element);

        // these templates were a lot more fun at the time of writing them
        template<typename T>
        void setByIndex(size_t index, T&& value)
        {
            ASSERT(index > 0 && index < m_layout->size(), "Index out of range");
            //ASSERT(_layout.get_by_index(index).get_size() == sizeof(T), "Vertex parameter input memory size mismatch. \n [SEMANTIC: " + _layout.get_by_index(index).get_semantic() + "]");

            *(T*)m_elements[m_layout->get(index).offset()] = value;
        }

        template<typename T>
        void setByIndex(size_t index, std::initializer_list<T>&& values)
        {
            ASSERT(index > 0 && index < m_layout->size(), 
                "Index out of range");
            ASSERT(m_layout->get(index).size() == sizeof(T) * values.size(), 
                "Vertex parameter input memory size mismatch. \n [SEMANTIC: " << m_layout->get(index).semantic() << "]");
            *(T**)m_elements[m_layout->get(index).offset()] = values.begin();
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
        std::shared_ptr<VertexLayout> m_layout;
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
        //std::shared_ptr<VertexLayout> m_layout;
        ID3D11Buffer* m_d3d11Buffer;
    };

    // TODO: dynamic vertex buffer class
        //
}
