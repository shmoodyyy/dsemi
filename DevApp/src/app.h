#ifndef DEVAPP_APP
#define DEVAPP_APP
#include <dsemi/Dsemi.h>
#include "scenes/testscene2d.h"
#include <dsemi/graphics/color.h>
#include <dsemi/graphics/render2d.h>
#include <dsemi/graphics/api_include.h>
#include <dsemi/graphics/color.h>

#include <dsemi/graphics/resources/resource_include.h>

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

    void initDX();
    void drawTriangle();

    TestScene2D m_testScene;
    std::shared_ptr<dsemi::Window> m_window;
    std::shared_ptr<dsemi::Window> m_windowSecondary;

	dsemi::graphics::Device*		_device;
	ComPtr<ID3D11Device>			_dx_device;
	ComPtr<ID3D11DeviceContext>		_dx_context;
	ComPtr<IDXGISwapChain>			_dx_swap_chain;
	ComPtr<ID3D11Buffer>			_vertex_buffer;
	ComPtr<ID3D11Buffer>			_view_const_buffer;
	ComPtr<ID3D11VertexShader>		_vertex_shader;
	ComPtr<ID3D11PixelShader>		_pixel_shader;
	ComPtr<ID3D11RenderTargetView>	_rt_view;
	ComPtr<ID3D11InputLayout>		_input_layout;
	D3D11_VIEWPORT					_viewport;
	dsemi::colorf					_clear_color;

	dsemi::graphics::RenderTarget _render_target;
	//dsemi::graphics::viewport _viewport;
	//dsemi::graphics::viewport _viewport;

	std::unique_ptr<dsemi::graphics::VertexBuffer> _vbuf;

	struct vertex
	{
		int x;
		int y;
	};
};

#endif
