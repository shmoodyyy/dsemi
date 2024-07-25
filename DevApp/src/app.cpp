#include "app.h"
#include <iostream>
#include <dxgidebug.h>

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
    _dx_device = _device->get_dx_device();
    _dx_context = _device->get_context();
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
    drawTriangle();
}

auto DevApp::onWindowClose(dsemi::WindowCloseEvent &e) -> bool
{
    m_isRunning = false;
    return true;
}

auto DevApp::onWindowResize(dsemi::WindowResizeEvent& e) -> bool
{
    if (_dx_context) {
        HRESULT hr;
        _viewport.Width  = m_window->getWidth();
        _viewport.Height = m_window->getHeight();
        _dx_context->RSSetViewports(1u, &_viewport);

        // update constant buffer for view dimensions
        float view_w = m_window->getWidth();
        float view_h = m_window->getHeight();
        std::vector<float> view_scale_2d = {
            view_w,
            view_h
        };
        // send new view dimensions to GPU memory
        _dx_context->UpdateSubresource(
            _view_const_buffer.Get(),
            0u,
            NULL,
            view_scale_2d.data(),
            sizeof(float) * view_scale_2d.size(),
            0u
        );
        //_dx_context->VSSetConstantBuffers(0u, 1u, _view_const_buffer.GetAddressOf());
    }
    return true;
}

void logDirectX_thread() // absolutely unconcerned with thread-safety or race conditions here
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
    _clear_color = 0.0f;
    HRESULT hr;
    // =======================================================
    //		CREATE RENDER TARGET VIEW
    // =======================================================
    GFX_LOG_DEBUG(L"Created DX11 RenderTargetView.");

    // =======================================================
    //		CREATE VIEW PORT
    // =======================================================
    unsigned int vp_width  = m_window->getWidth();
    unsigned int vp_height = m_window->getHeight();
    _viewport.TopLeftX = 0u;
    _viewport.TopLeftY = 0u;
    _viewport.Width    = vp_width;
    _viewport.Height   = vp_height;
    _viewport.MinDepth = 0u;
    _viewport.MaxDepth = 1u;

    // =======================================================
    //		CREATE VERTEX & PIXEL SHADERS
    // =======================================================

    UINT shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifndef _NDEBUG
    shader_flags |= D3DCOMPILE_DEBUG;
#endif

    ComPtr<ID3DBlob> vs_blob  = nullptr;
    ComPtr<ID3DBlob> ps_blob  = nullptr;
    ComPtr<ID3DBlob> err_blob = nullptr;

    // read from file
    GFX_THROW_FAILED(D3DReadFileToBlob(L"shaders/default_vs.cso", &vs_blob));
    // create in gpu
    GFX_THROW_FAILED(_dx_device->CreateVertexShader(
        vs_blob->GetBufferPointer(),
        vs_blob->GetBufferSize(),
        nullptr,
        &_vertex_shader
    ));

    // read from file
    GFX_THROW_FAILED(D3DReadFileToBlob(L"shaders/default_ps.cso", &ps_blob));
    // create in gpu
    GFX_THROW_FAILED(_dx_device->CreatePixelShader(
                ps_blob->GetBufferPointer(),
                ps_blob->GetBufferSize(),
                nullptr,
                &_pixel_shader
                ));

    // =======================================================
    //		CREATE INPUT LAYOUT
    // =======================================================
    // input parameters
    const D3D11_INPUT_ELEMENT_DESC input_elements[] = {
        {"Position", 0u, DXGI_FORMAT_R32G32_SINT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u}
    };
    // create in gpu
    GFX_THROW_FAILED(_dx_device->CreateInputLayout(
                input_elements,
                1u,
                vs_blob->GetBufferPointer(),
                vs_blob->GetBufferSize(),
                &_input_layout
                ));

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

    GFX_THROW_FAILED(_dx_device->CreateBuffer(&vmcbd, &vmsrd, &_view_const_buffer));

    // =======================================================
    //		CREATE VERTEX BUFFER
    // =======================================================
    dsemi::graphics::vertex_layout layout;
    layout.append("Position", dsemi::graphics::shader_data_type::SINT2);

    dsemi::graphics::vertex_array vertices(layout);
    int w = m_window->getWidth() / 2;
    int h = m_window->getHeight() / 2;
    vertices.emplace_back(0, h).emplace_back(w, -h).emplace_back(-w, -h);
    _vbuf = std::make_unique<dsemi::graphics::vertex_buffer>(_device, vertices);

    uint32_t strides = 8u;
    uint32_t offsets = 0u;
    _dx_context->VSSetShader(_vertex_shader.Get(), nullptr, 0u);
    _dx_context->PSSetShader(_pixel_shader.Get(), nullptr, 0u);
    _dx_context->IASetInputLayout(_input_layout.Get());
    m_window->getRenderTarget()->set();
    _dx_context->RSSetViewports(1u, &_viewport);
    _dx_context->VSSetConstantBuffers(0u, 1u, _view_const_buffer.GetAddressOf());
    _vbuf->bind();

    std::thread(logDirectX_thread).detach();
}

void DevApp::drawTriangle()
{
    m_window->getRenderTarget()->clear();
    m_window->getRenderTarget()->set();
    _dx_context->Draw(_vbuf->get_count(), 0u);

    HRESULT hr;
    if (FAILED(hr = m_window->getSwapChain()->m_swapChain->Present(0, 0)))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            throw HRESULT_EXCEPTION(_dx_device->GetDeviceRemovedReason());
        }
        else
        {
            GFX_THROW_FAILED(hr);
        }
    }
}
