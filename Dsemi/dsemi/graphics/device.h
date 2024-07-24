#ifndef GRAPHICS_DEVICE_H
#define GRAPHICS_DEVICE_H

#include "dsemi/graphics/api_include.h"
#include "dsemi/graphics/render_target.h"
#include "dsemi/graphics/render_context.h"
#include "dsemi/graphics/resources/resource_include.h"
#include "dsemi/graphics/swapchain.h"

#include "dsemi/graphics/color.h"

namespace dsemi::graphics {
    class Device {
    public:
        inline static Device& get() {
            static Device _instance;
            return _instance;
        }

        auto get_dx_factory() -> IDXGIFactory* const { return _dxgi_factory; }
        auto get_dx_device() -> ID3D11Device* const { return _dx_device.Get(); }
        auto get_context() -> ID3D11DeviceContext* const { return _dx_context.Get(); }

        auto createSwapChain() -> std::shared_ptr<SwapChain>;
        void create_vertex_buffer(vertex_buffer* vertex_buffer);
        void create_index_buffer(index_buffer* index_buffer);
        void create_vertex_shader(vertex_shader* vertex_shader, vertex_shader_desc desc);
        void create_fragment_shader(fragment_shader* fragment_shader, fragment_shader_desc desc);
        void create_render_target(render_target* render_target, ID3D11Resource* source); // temp pointer for until i implement own basic resource class
        void bind_vertex_buffer(const vertex_buffer* vertex_buffer);
        //void bind_input_layout(input_layout* layout);
        void bind_vertex_shader(vertex_shader* vertex_shader);
        void bind_fragment_shader(fragment_shader* fragment_shader);
        void bind_render_target(render_target* render_target);
        //void create_triangle_resources();

    private:
        Device();
        ~Device();
        Device(const Device& other) = delete;
        Device& operator=(const Device& other) = delete;

        void _initialize_impl();
        IDXGIFactory* _get_dxgi_factory();

        color32 _clear_color;
        float* _clear_color_float;
        ComPtr<ID3D11Device>        _dx_device;
        ComPtr<ID3D11DeviceContext> _dx_context;

        IDXGIFactory* _dxgi_factory;

        ComPtr<ID3D11RasterizerState> _dx_rasterizer_state;

        // TODO: move below code to graphics_render_target
        ComPtr<IDXGISwapChain> _dxgi_swapchain;
        ComPtr<ID3D11RenderTargetView> _dx_render_target_view;

        D3D11_VIEWPORT _dx_viewport;
    };
}

#endif
