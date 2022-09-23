#ifndef GRAPHICS_DEVICE_H
#define GRAPHICS_DEVICE_H

#include "dsemi/graphics/api_include.h"
#include "dsemi/graphics/render_target.h"
#include "dsemi/graphics/render_context.h"
#include "dsemi/graphics/resources/resource_include.h"

#include "dsemi/graphics/color.h"

namespace dsemi {
	class window;

	namespace graphics {
		// singleton class to access graphics driver for resource and context creation
		class device {
			friend class core;
		private:
			device() {}
			~device();
			device(const device& other) = delete;
			device& operator=(const device& other) = delete;

			static device* _instance;

		public:
			inline device* get() {
				ASSERT(_instance != nullptr, "Attempting to access graphics API device before having initialized it.");
				return _instance;
			}

			static void initialize();

			void create_vertex_buffer(vertex_buffer* vertex_buffer, vertex_buffer_desc desc);
			void create_index_buffer(gfx_index_buffer* index_buffer, index_buffer_desc desc);
			void create_vertex_shader(vertex_shader* vertex_shader, vertex_shader_desc desc);
			void create_fragment_shader(fragment_shader* fragment_shader, fragment_shader_desc desc);
			void create_render_target(render_target* render_target, ID3D11Resource* source); // temp pointer for until i implement own basic resource class

			render_context create_context(dsemi::window* target_window);

			void bind_vertex_buffer(vertex_buffer* vertex_buffer);
			void bind_input_layout(input_layout* layout);
			void bind_vertex_shader(vertex_shader* vertex_shader);
			void bind_fragment_shader(fragment_shader* fragment_shader);
			void bind_render_target(render_target* render_target);

			//void create_triangle_resources();


		private:
			void _initialize_impl();


		private:
			// TODO: render targets [OS window and textures as targets for drawing pipeline]

			color32 _clear_color;
			float* _clear_color_float;


		public:
			IDXGIFactory*        get_dx_factory() const noexcept { return _dxgi_factory; };
			ID3D11Device*        get_dx_device() const noexcept  { return _dx_device.Get(); }
			ID3D11DeviceContext* get_context() const noexcept { return _dx_context.Get(); }

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
		};
	}
}

#endif