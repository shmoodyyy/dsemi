#include "dsemi/graphics/swapchain.h"
#include "dsemi/graphics/device.h"
#include "dsemi/core/window.h"

dsemi::graphics::SwapChain::SwapChain(Window& wnd)
{
    m_clearColor.red = 0.0f;
    m_clearColor.green = 0.0f;
    m_clearColor.blue = 0.0f;
    auto& device = Device::get();
    DXGI_SWAP_CHAIN_DESC sd = {{0}};
    sd.BufferDesc.Width = wnd.getWidth();
    sd.BufferDesc.Height = wnd.getHeight();
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count = 1u;
    sd.SampleDesc.Quality = 0u;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 2u;
    sd.OutputWindow = wnd.getHwnd();
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    HRESULT hr;
    auto dxgi_factory = device.get_dx_factory();
    GFX_THROW_FAILED(dxgi_factory->CreateSwapChain(
        device.get_dx_device(),
        &sd,
        &m_swapChain
    ));
    setFrameBufferTexture();
    setRenderTargetView();
    GFX_LOG_DEBUG(L"created new swapchain for test window");
}

dsemi::graphics::SwapChain::~SwapChain()
{
}

auto dsemi::graphics::SwapChain::resize(unsigned width, unsigned height) -> ErrNo
{
    if (!m_swapChain)
        return badMemory;
    // release all render target views holding reference to our buffer
    m_renderTexture = nullptr;
    m_renderTargetView = nullptr;
    DXGI_SWAP_CHAIN_DESC sd;
    m_swapChain->GetDesc(&sd);
    m_swapChain->ResizeBuffers(sd.BufferCount, 0, 0, DXGI_FORMAT_UNKNOWN, sd.Flags);
    setFrameBufferTexture();
    setRenderTargetView();
    return ok;
}

auto dsemi::graphics::SwapChain::present() -> ErrNo
{
    m_swapChain->Present(0, 0);
    return ok;
} 
 
void dsemi::graphics::SwapChain::setFrameBufferTexture()
{
    HRESULT hr;
    GFX_THROW_FAILED(m_swapChain->GetBuffer(
        0u,
        __uuidof(ID3D11Texture2D),
        &m_renderTexture
    ));
}

void dsemi::graphics::SwapChain::setRenderTargetView()
{
    HRESULT hr;
    GFX_THROW_FAILED(Device::get().get_dx_device()->CreateRenderTargetView(
        m_renderTexture.Get(),
        0u,
        &m_renderTargetView
    ));
}
