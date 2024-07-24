#include "dsemi/graphics/swapchain.h"
#include "dsemi/graphics/device.h"
#include "dsemi/core/window.h"

dsemi::graphics::SwapChain::SwapChain(Window& wnd)
{
    auto& device = Device::get();
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferDesc.Width = 1280;
    sd.BufferDesc.Height = 720;
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
    GFX_LOG_DEBUG(L"created new swapchain for test window");
}

dsemi::graphics::SwapChain::~SwapChain()
{
}

auto dsemi::graphics::SwapChain::resize(unsigned width, unsigned height) -> ErrNo
{
    HRESULT hr;
    if (!m_swapChain)
        return badMemory;
    DXGI_SWAP_CHAIN_DESC sd;
    m_swapChain->GetDesc(&sd);
    m_swapChain->ResizeBuffers(sd.BufferCount, 0u, 0u, DXGI_FORMAT_UNKNOWN, sd.Flags);
    ComPtr<ID3D11Texture2D> framebuf = nullptr;
    GFX_THROW_FAILED(m_swapChain->GetBuffer(
                0u,
                __uuidof(ID3D11Texture2D),
                &framebuf
                ));
    return ok;
}

auto dsemi::graphics::SwapChain::present() -> ErrNo
{
    m_swapChain->Present(0, 0);
    return ok;
}
