#ifndef DSEMI_GRAPHICS_VERTEXARRAY
#define DSEMI_GRAPHICS_VERTEXARRAY
#include <dsemi/graphics/resources/vertexlayout.h>
#include <memory>

namespace dsemi::graphics
{
    class VertexLayout;
    class VertexArray
    {
    public:
        VertexArray(std::shared_ptr<VertexLayout> layout, size_t capacity = 0u);
        ~VertexArray();

        auto data() const -> const void* const;
        auto size() const -> const size_t;
        auto capacity() const -> const size_t;
        auto stride() const -> const size_t;
        auto byteWidth() const -> const size_t;
        auto layout() const -> std::shared_ptr<VertexLayout>;

        template<typename ...ParamTypes>
        VertexArray& emplace(ParamTypes&&... params)
        {
            //ASSERT(sizeof...(ParamTypes) == m_layout->size(), "Vertex parameters don't match the VertexArray's element layout.");
            if (m_size+1 > m_capacity)
                resize((m_size+1) * 2);
            std::uintptr_t ptr = (std::uintptr_t)(m_data) + (m_size * m_layout->stride());
            emplace_setParam(0, ptr, std::forward<ParamTypes>(params)...);
            ++m_size;
            //emplace_finish();
            return *this;
        }

    private:
        void resize(std::size_t size);

        void emplace_finish();

        // 10.08.2024 TODO: more rigurous input validation, checking the whole size of everything before even checking
        //                  capacity for new memory alloc
        template<typename T, typename... Tyepes>
        void emplace_setParam(unsigned index, std::uintptr_t ptr, T param_first, Tyepes... param_rest)
        {
            emplace_setParam(index, ptr, param_first);
            emplace_setParam(index+1, ptr+sizeof(T), param_rest...);
        }

        template<typename T>
        void emplace_setParam(unsigned index, std::uintptr_t ptr, T param)
        {
            //bool allowMutation = sizeof(T) == m_layout->get(index).size();
            //ASSERT(allowMutation, "parameter arg is wrong bytesize");
            //if (!allowMutation)
            //    return;
            T& val = *reinterpret_cast<T*>(ptr);
            val = param;
            /*_bytes.resize(_bytes.size() + sizeof(T));
            auto tail = _bytes.end() - sizeof(T);*/
            //*(reinterpret_cast<T*>(&(*tail))) = param;
        }

        void* m_data;
        std::size_t m_size;
        std::size_t m_capacity;
        std::shared_ptr<VertexLayout> m_layout;
        bool m_emplaceValid;
    };
}

#endif
