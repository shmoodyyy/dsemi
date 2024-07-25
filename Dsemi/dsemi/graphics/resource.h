#ifndef DSEMI_GRAPHICS_RESOURCE
#define DSEMI_GRAPHICS_RESOURCE
#include "api_include.h"
#include "bindable.h"

namespace dsemi::graphics
{
    class Resource : public Bindable
    {
    public:
        Resource();
        ~Resource();

        virtual void bind() override;

#if defined (_WIN32)
    private:
        ComPtr<ID3D11Resource> m_resource;
#endif
    };
}

#endif
