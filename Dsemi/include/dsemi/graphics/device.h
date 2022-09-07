#ifndef GRAPHICS_DEVICE_H
#define GRAPHICS_DEVICE_H

#include "dsemi/graphics/api_include.h"
#include "dsemi/graphics/render_target.h"
#include "dsemi/graphics/swap_chain.h"
#include "dsemi/graphics/resources/resource_include.h"

#include "dsemi/graphics/color.h"

namespace dsemi {

	class gfx_device {
	public:
		gfx_device() {}
		~gfx_device();
		gfx_device(const gfx_device& other) = delete;
		gfx_device& operator=(const gfx_device& other) = delete;

		void initialize();

		/*====================
		// resource creation
		======================*/

		// general
		void create_vertex_buffer(gfx_vertex_buffer* vertex_buffer, gfx_vertex_buffer_desc desc);
		void create_index_buffer(gfx_index_buffer* index_buffer, gfx_index_buffer_desc desc);
		void create_vertex_shader(gfx_vertex_shader* vertex_shader, gfx_vertex_shader_desc desc);
		void create_fragment_shader(gfx_fragment_shader* fragment_shader, gfx_fragment_shader_desc desc);
		void create_swap_chain(gfx_swap_chain* swap_chain);
		void create_render_target(gfx_render_target* render_target, ID3D11Resource* source); // temp pointer for until i implement own basic resource class

		// 3D

		// 2D

		//void create_render_target_2d();

		/*===================
		// resource binding
		=====================*/
		void bind_vertex_buffer(gfx_vertex_buffer* vertex_buffer);

		//void bind_input_layout(gfx_input_layout* layout);
		void bind_vertex_shader(gfx_vertex_shader* vertex_shader);
		void bind_fragment_shader(gfx_fragment_shader* fragment_shader);
		void bind_render_target(gfx_render_target* render_target);

		/*===================
		// state management
		=====================*/
		void set_clear_color(float r, float g, float b, float a);
		void set_clear_color(color32 rgba);


		/*=============
		// draw calls
		===============*/
		void clear();
		void draw(unsigned int num_vertices);
		void draw_triangle();
		void present();


		/*===================
		// debugging
		=====================*/
		//void create_triangle_resources();


	private:
		void _initialize_impl();


	private:
		// TODO: render targets [OS window and textures as targets for drawing pipeline]
		gfx_render_target* _active_render_target;
		gfx_swap_chain _active_swap_chain;

		color32 _clear_color;
		float* _clear_color_float;


#ifdef GFX_USING_DX11
	private:
		IDXGIFactory* _get_dxgi_factory();

	private:
		ComPtr<ID3D11Device>        _dx_device;
		ComPtr<ID3D11DeviceContext> _dx_context;

		IDXGIFactory* _dxgi_factory;

		ComPtr<ID3D11RasterizerState> _dx_rasterizer_state;

		// TODO: move below code to graphics_render_target
		ComPtr<IDXGISwapChain> _dxgi_swapchain;
		ComPtr<ID3D11RenderTargetView> _dx_render_target_view;

		D3D11_VIEWPORT _dx_viewport;

#endif
	};
}

#endif