#ifndef HEADER_DSEMI_ECS_COMPONENTS_INSTANCEDPOLYGON2D
#define HEADER_DSEMI_ECS_COMPONENTS_INSTANCEDPOLYGON2D

#include "dsemi/ecs/component.h"
#include "dsemi/graphics/color.h"

namespace dsemi {
	namespace ecs {
		struct polygon2d_instanced : public icomponent<polygon2d_instanced>
		{
		public:
			polygon2d_instanced();


			void* vertices;
		};
	}
}

#endif