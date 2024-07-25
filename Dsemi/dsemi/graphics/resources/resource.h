#ifndef DSEMI_GRAPHICS_RESOURCE
#define DSEMI_GRAPHICS_RESOURCE
#include "dsemi/graphics/api_include.h"
#include "dsemi/graphics/bindable.h"

namespace dsemi::graphics
{
    class Resource : public Bindable
    {
    public:
        Resource();
        ~Resource();

        virtual void bind() override;

#if defined (_WIN32)
    protected:
        ComPtr<ID3D11Resource> m_resource;
#endif
    };
}

#endif
