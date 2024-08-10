#include "app.h"
#include <iostream>
#include <dxgidebug.h>
#include <dsemi/core/input.h>

#pragma comment(lib, "dxguid.lib")

DevApp::DevApp()
{
}

DevApp::~DevApp()
{
}

void DevApp::onInit()
{
    m_activeScene = &m_testScene;
    m_window = std::make_shared<dsemi::Window>(1280, 720, "gfx window");
    m_window->setEventCallback(BIND_EVENT(DevApp::onEvent));
    m_window->getSwapChain()->m_swapChain = m_window->getSwapChain()->m_swapChain;

    _device = &dsemi::graphics::Device::get();
    initDX();
}

void DevApp::onEvent(dsemi::ievent& e)
{
    dsemi::event_dispatcher dispatcher(e);
    dispatcher.dispatch<dsemi::WindowCloseEvent>(BIND_EVENT(DevApp::onWindowClose));
    dispatcher.dispatch<dsemi::WindowResizeEvent>(BIND_EVENT(DevApp::onWindowResize));
}

void DevApp::onUpdate(const float dt)
{
    auto moveViewport = [&](bool vertical, char keycode, float vel)
    {
        if (dsemi::Input::Instance()->GetKeyDown(keycode)) {
            if (vertical)
                m_viewport.setY(m_viewport.getY() + vel * dt);
            else
                m_viewport.setX(m_viewport.getX() + vel * dt);
        }
    };
    moveViewport(true, 'W', 500.0f);
    moveViewport(false, 'A', 500.0f);
    moveViewport(true, 'S', -500.0f);
    moveViewport(false, 'D', -500.0f);
    m_viewport.bind();

    drawTriangle();
}

auto DevApp::onWindowClose(dsemi::WindowCloseEvent &e) -> bool
{
    m_isRunning = false;
    return true;
}

auto DevApp::onWindowResize(dsemi::WindowResizeEvent& e) -> bool
{
    if (!_device->getContext())
        return true;
    m_viewport.setSize(m_window->getWidth(), m_window->getHeight());
    m_viewport.bind();

    // update constant buffer for view dimensions
    float view_w = m_window->getWidth();
    float view_h = m_window->getHeight();
    std::vector<float> view_scale_2d = {
        view_w,
        view_h
    };
    // send new view dimensions to GPU memory
    _device->getContext()->UpdateSubresource(
        _view_const_buffer.Get(),
        0u,
        NULL,
        view_scale_2d.data(),
        sizeof(unsigned) * view_scale_2d.size(),
        0u
    );
    //_device->getContext()->VSSetConstantBuffers(0u, 1u, _view_const_buffer.GetAddressOf());
    return true;
}

auto DevApp::onKeyDown(dsemi::KeyDownEvent& e) -> bool
{
    e.handled = true;
    return true;
}

void logDirectX_thread() // absolutely unconcerned with race conditions here
{
    // holy shit i love win32 who needs manpages anyways
    using namespace dsemi::graphics;
    typedef HRESULT (WINAPI* DXGIGetDebugInterface)(REFIID, void**);
    ComPtr<IDXGIInfoQueue> dxgiDebugQueue;
    auto handle = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
        reinterpret_cast<void*>(GetProcAddress(handle, "DXGIGetDebugInterface"))
    );
    DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &dxgiDebugQueue);
    dxgiDebugQueue->ClearStorageFilter(DXGI_DEBUG_ALL);
    dxgiDebugQueue->ClearRetrievalFilter(DXGI_DEBUG_ALL);
    while (true) {
        for (unsigned i = 0; i < dxgiDebugQueue->GetNumStoredMessages(DXGI_DEBUG_ALL); ++i) {
            SIZE_T len = 0;
            dxgiDebugQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &len);
            DXGI_INFO_QUEUE_MESSAGE* msg = (DXGI_INFO_QUEUE_MESSAGE*)malloc(len);
            dxgiDebugQueue->GetMessage(DXGI_DEBUG_ALL, i, msg, &len);
            if (msg->Producer == DXGI_DEBUG_DX)
                std::cout << "D3D11 [";
            else if (msg->Producer == DXGI_DEBUG_DXGI)
                std::cout << "DXGI [";
            std::cout << msg->Severity << "]: " << msg->pDescription << '\n';
            free(msg);
        }
        dxgiDebugQueue->ClearStoredMessages(DXGI_DEBUG_ALL);
    }
}

void DevApp::initDX()
{
    std::thread(logDirectX_thread).detach();
    HRESULT hr;
    // =======================================================
    //		CREATE RENDER TARGET VIEW
    // =======================================================
    // created in dsemi::graphics::SwapChain
    // GFX_LOG_DEBUG(L"Created DX11 RenderTargetView.");

    // =======================================================
    //		CREATE VIEW PORT
    // =======================================================
    unsigned int vp_width  = m_window->getWidth();
    unsigned int vp_height = m_window->getHeight();
    m_viewport.setSize(vp_width, vp_height);

    // =======================================================
    //		CREATE VERTEX & PIXEL SHADERS
    // =======================================================
    UINT shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef DSEMI_DEBUG
    shader_flags |= D3DCOMPILE_DEBUG;
#endif

    // =======================================================
    //		CREATE CONSTANT BUFFER (2D VIEW MATRIX)
    // =======================================================
    // GOAL: map coordinates to pixels when zoom is at 1.0f (default)
    // Matrix for 2D Viewport
    // [ 1 0 ] -> [ 1/w  0   ]
    // [ 0 1 ] -> [ 0    1/h ]

    float zoom_scale = 1.0f;
    float view_w = m_window->getWidth();
    float view_h = m_window->getHeight();
    std::vector<float> view_scale_2d = {
        view_w,
        view_h
    };

    D3D11_BUFFER_DESC vmcbd   = {};
    vmcbd.ByteWidth           = view_scale_2d.size() * sizeof(float) * 2.0f;
    vmcbd.Usage               = D3D11_USAGE_DEFAULT;
    vmcbd.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
    vmcbd.CPUAccessFlags      = 0u;
    vmcbd.MiscFlags           = 0u;
    vmcbd.StructureByteStride = 0u;

    D3D11_SUBRESOURCE_DATA vmsrd = {};
    vmsrd.pSysMem = view_scale_2d.data();

    GFX_THROW_FAILED(_device->getDxDevice()->CreateBuffer(&vmcbd, &vmsrd, &_view_const_buffer));

    // =======================================================
    //		CREATE VERTEX BUFFER
    // =======================================================

    // 09.08.2024: i think the vertex shader should own the layout, it seems to make the most sense as of now
    auto layout = std::make_shared<dsemi::graphics::VertexLayout>();
    layout->append("Position", dsemi::graphics::ShaderDataType::SINT2);

    m_vertexShader = std::make_shared<dsemi::graphics::VertexShader>("default_vs", layout);

    m_vertices = std::make_shared<dsemi::graphics::VertexArray>(layout);
    int w = m_window->getWidth() / 2;
    int h = m_window->getHeight() / 2;
    int scale = 100;
    m_vertices
        ->emplace(-2*scale,scale*0)
        .emplace(-1*scale,scale*1)
        .emplace(0*scale,scale*0)

        .emplace(0*scale,scale*0)
        .emplace(1*scale,scale*1)
        .emplace(2*scale,scale*0)

        .emplace(-2*scale,scale*0)
        .emplace(2*scale,scale*0)
        .emplace(0*scale,scale*-2);

    _vbuf = std::make_unique<dsemi::graphics::VertexBuffer>(*m_vertices);

    m_fragmentShader = std::make_shared<dsemi::graphics::FragmentShader>("default_ps");

    m_vertexShader->bind();
    m_fragmentShader->bind();
    m_window->getRenderTarget()->set();
    m_viewport.bind();
    _device->getContext()->VSSetConstantBuffers(0u, 1u, _view_const_buffer.GetAddressOf());
    _vbuf->bind();

}

void DevApp::drawTriangle()
{
    m_window->getRenderTarget()->clear();
    m_window->getRenderTarget()->set();
    _device->getContext()->Draw(_vbuf->size(), 0u);

    HRESULT hr;
    if (FAILED(hr = m_window->getSwapChain()->m_swapChain->Present(0, 0)))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            throw HRESULT_EXCEPTION(_device->getDxDevice()->GetDeviceRemovedReason());
        }
        else
        {
            GFX_THROW_FAILED(hr);
        }
    }
}
