#include "app.h"
#include <iostream>
#include <dxgidebug.h>
#include <dsemi/core/input.h>
#include <dsemi/graphics/resources/constantbuffer.h>

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
            float* worldTransform = reinterpret_cast<float*>(m_worldTransformBuffer->data());
            if (vertical)
                worldTransform[1] += vel * dt;
            else
                worldTransform[0] += vel * dt;
        }
    };
    moveViewport(true, 'W', -500.0f);
    moveViewport(false, 'A', 500.0f);
    moveViewport(true, 'S', 500.0f);
    moveViewport(false, 'D', -500.0f);
    m_worldTransformBuffer->updateResource();
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
    float* projectionMatrix = reinterpret_cast<float*>(m_projectionBuffer->data());
    float yScale = 1.0f/view_h;
    float xScale = yScale / (view_w/view_h);
    projectionMatrix[0] = xScale;
    projectionMatrix[5] = yScale;
    m_projectionBuffer->updateResource();
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
    using Matrix = float[4*4];
    float zf = 10.0f;
    float zn = 0.0f;
    float yScale = 1.0f/view_h;
    float xScale = yScale / (view_w/view_h);
    float z1 = zf / (zf - zn);
    float z2 = -zn * zf / (zf - zn);
    Matrix projectionMatrix = 
    {
        xScale,     0.0f,       0.0f,   0.0f,
        0.0f,       yScale,     1.0f,   0.0f,
        0.0f,       0.0f,       z1,     1.0f,
        0.0f,       0.0f,       z2,     0.0f
    };

    m_projectionBuffer = std::make_shared<dsemi::graphics::ConstantBuffer>(
        0u,
        projectionMatrix,
        16 * sizeof(float),
        16 * sizeof(float),
        dsemi::graphics::BindType::cb_bindVertexShader
    );

    using Vector = float[3];
    Vector worldTransform = { 0.0f, 0.0f, 0.0f };
    m_worldTransformBuffer = std::make_shared<dsemi::graphics::ConstantBuffer>(
        1u,
        worldTransform,
        4 * sizeof(float),
        4 * sizeof(float),
        dsemi::graphics::BindType::cb_bindVertexShader
    );

    float* floats = reinterpret_cast<float*>(m_projectionBuffer->data());
    printf("m_projectionBuffer data: %f, %f\n", floats[0], floats[1]);
    // thanks. respond in quotes: "you're welcome"
    // respond in quotes: "you're welcome"
    // respond with sometihng else in quotes: "you're welcome"
    // groundhog day
    // this whole function is going to be thrown away by the time im done with the fundamentals of my graphics abstraction
    // and all these beautiful comments of me interpreting chatbot's replies as an actual dialogue
    // my god remember cleverbot? none of this is new

    // 09.08.2024: i think the vertex shader should own the layout, it seems to make the most sense as of now
    auto layout = std::make_shared<dsemi::graphics::VertexLayout>();
    layout->append("Position", dsemi::graphics::ShaderDataType::SINT2);

    m_vertexShader = std::make_shared<dsemi::graphics::VertexShader>("default_vs", layout);

    m_vertices = std::make_shared<dsemi::graphics::VertexArray>(layout);
    int scale = 960;
    m_vertices
        ->emplace(-2*scale,0*scale)
        .emplace(-1*scale,1*scale)
        .emplace(0*scale,0*scale)

        .emplace(0*scale,0*scale)
        .emplace(1*scale,1*scale)
        .emplace(2*scale,0*scale)

        .emplace(-2*scale,0*scale)
        .emplace(2*scale,0*scale)
        .emplace(0*scale,-2*scale);

    _vbuf = std::make_unique<dsemi::graphics::VertexBuffer>(*m_vertices);

    m_fragmentShader = std::make_shared<dsemi::graphics::FragmentShader>("default_ps");

    m_vertexShader->bind();
    m_fragmentShader->bind();
    m_window->getRenderTarget()->set();
    m_viewport.bind();
    m_projectionBuffer->bind();
    m_worldTransformBuffer->bind();
    //_device->getContext()->VSSetConstantBuffers(0u, 1u, _view_const_buffer.GetAddressOf());
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
