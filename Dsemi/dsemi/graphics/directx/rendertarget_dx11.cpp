#include "dsemi/graphics/rendertarget.h"
#include "dsemi/graphics/device.h"

dsemi::graphics::RenderTarget::RenderTarget()
    : m_renderTexture(nullptr)
    , m_renderTargetView(nullptr)
{
}

void dsemi::graphics::RenderTarget::clear()
{
    Device::get().get_context()->ClearRenderTargetView(m_renderTargetView.Get(), m_clearColor.as_array());
}

void dsemi::graphics::RenderTarget::set()
{
    Device::get().get_context()->OMSetRenderTargets(1u, m_renderTargetView.GetAddressOf(), nullptr);
}
