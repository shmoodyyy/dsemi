#ifndef HEADER_DSEMI_ECS_COMPONENTS_GFX_POLYGON2D
#define HEADER_DSEMI_ECS_COMPONENTS_GFX_POLYGON2D

#include "Dsemi/ecs/Component.h"
#include "Dsemi/graphics/graphics.h"
#include <vector>

namespace dsemi {
	namespace ecs {
		struct vertex_2d
		{
			vertex_2d()
				: x(0.0f), y(0.0f)
			{}
			vertex_2d(float x, float y)
				: x(x), y(y)
			{}
			float x, y;
		};

		struct polygon2d : dsemi::ecs::icomponent<polygon2d>
		{
			polygon2d()
				: vertices(0)
			{}
			std::vector<vertex_2d> vertices;
		};
	}
}

#endif