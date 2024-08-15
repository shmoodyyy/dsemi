#ifndef DSEMI_GRAPHICS_CONSTANTBUFFER
#define DSEMI_GRAPHICS_CONSTANTBUFFER
#include <dsemi/graphics/api_include.h>
#include <dsemi/graphics/bindable.h>

namespace dsemi::graphics
{
    enum class BindType
    {
        cb_bindVertexShader,
        cb_bindFragmentShader,
    };

    // 11.08.2024 TODO: design this class to constrain types, values
    //                  etc to always have the same bytesize of input data
    // 14.08.2024 TODO: my wording is booty cheeks
    class ConstantBuffer : public Bindable
    {
    public:
        ConstantBuffer(unsigned bindSlot, void* data, size_t size, size_t stride, BindType bindType);
        template<typename T>
        ConstantBuffer(unsigned bindSlot, T data, BindType bindType)
            : ConstantBuffer(bindSlot, new T(data), sizeof(T), bindType) 
        {}
        ~ConstantBuffer();

        auto data() -> void*;
        template<typename T>
        auto data() -> T& { return *reinterpret_cast<T*>(data()); }

        void setBindType(BindType bindType);
        auto getBindType() const -> BindType;
        void updateResource();

        virtual void bind();

    private:
        unsigned m_bindSlot;
        BindType m_bindType;
        void* m_data;
        size_t m_size;
        size_t m_stride;
#ifdef _WIN32
        ComPtr<ID3D11Buffer> m_d3d11Buffer;
#endif
    };
}

#endif
