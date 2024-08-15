#ifndef DEVAPP_APP
#define DEVAPP_APP
#include <dsemi/Dsemi.h>
#include "scenes/testscene2d.h"
#include <dsemi/graphics/color.h>
#include <dsemi/graphics/render2d.h>
#include <dsemi/graphics/api_include.h>
#include <dsemi/graphics/color.h>
#include <dsemi/graphics/viewport.h>

#include <dsemi/graphics/resources/resource_include.h>
#include <dsemi/events/keyboardevent.h>

// temporary includes for writing the graphics abstractions
#include <dsemi/graphics/rendertarget.h>

class DevApp : public dsemi::application
{
public:
    DevApp();
    ~DevApp();

private:
    virtual void onInit() override;
    virtual void onEvent(dsemi::ievent& e) override;
    virtual void onUpdate(const float dt) override;

    auto onWindowClose(dsemi::WindowCloseEvent& e) -> bool;
    auto onWindowResize(dsemi::WindowResizeEvent& e) -> bool;
    auto onKeyDown(dsemi::KeyDownEvent& e) -> bool;

    void initDX();
    void drawTriangle();

    TestScene2D m_testScene;
    std::shared_ptr<dsemi::Window> m_window;
    std::shared_ptr<dsemi::Window> m_windowSecondary;

    dsemi::graphics::Device*        _device;
    std::shared_ptr<dsemi::graphics::ConstantBuffer> m_projectionBuffer;
    std::shared_ptr<dsemi::graphics::ConstantBuffer> m_worldTransformBuffer;
    ComPtr<ID3D11Buffer>            _view_const_buffer;
    std::shared_ptr<dsemi::graphics::VertexShader> m_vertexShader;
    std::shared_ptr<dsemi::graphics::VertexArray> m_vertices;
    std::shared_ptr<dsemi::graphics::FragmentShader> m_fragmentShader;
    dsemi::graphics::Viewport       m_viewport;

    dsemi::graphics::RenderTarget   _render_target;

    std::unique_ptr<dsemi::graphics::VertexBuffer> _vbuf;
};

#endif
