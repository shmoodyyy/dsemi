#ifndef HEADER_DSEMI_GRAPHICS_RENDERER
#define HEADER_DSEMI_GRAPHICS_RENDERER

#include "dsemi/graphics/api_include.h"
#include "dsemi/graphics/color.h"
#include "dsemi/math/vector2.h"
#include "device.h"

namespace dsemi 
{
	class window;

	namespace graphics 
	{
		class device;

		// this might end up being replaced with a more
		// elaborate pipeline design for forward/deferred
		// rendering options hooked up with ECS or
		// custom defined inputs
		class renderer
		{
		public:

			enum mode {
				IMMEDIATE_2D,
				IMMEDIATE_3D,
			} mode;

			//static renderer create(window* target_window);
			void clear(color32 color);
			void draw_triangle(Vector2f position);
			void present();

		protected:
			renderer(window* target_window);
			~renderer();
			renderer(const renderer&) = delete;
			renderer& operator=(const renderer&) = delete;



		private:
			window* _target_window;
			Device* _device;
		};
	}
}

#endif
