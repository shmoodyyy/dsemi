#include "dsemi/graphics/rendertarget.h"
#include "dsemi/graphics/device.h"

dsemi::graphics::RenderTarget::RenderTarget()
    : m_renderTexture(nullptr)
    , m_renderTargetView(nullptr)
{
    D3D11_RENDER_TARGET_VIEW_DESC rtvd;
    memset(&rtvd, 0, sizeof(rtvd));
    GFX_THROW_FAILED(Device::get().get_dx_device()->CreateRenderTargetView(
        framebuf.Get(),
        0u,
        &m_renderTargetView
    ));
}

void dsemi::graphics::RenderTarget::clear()
{
    Device::get().get_context()->ClearRenderTargetView(m_renderTargetView.Get(), m_clearColor.as_array());
}

void dsemi::graphics::RenderTarget::set()
{
    Device::get().get_context()->OMSetRenderTargets(1u, m_renderTargetView.GetAddressOf(), nullptr);
}
