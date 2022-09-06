#ifndef HEADER_DSEMI_ECS_COMPONENTS_CORE_TRANSFORM2D
#define HEADER_DSEMI_ECS_COMPONENTS_CORE_TRANSFORM2D

#include "dsemi/ecs/component.h"
#include "dsemi/math/vector2.h"

namespace dsemi {
	namespace ecs {
		struct transform2d : public icomponent<transform2d>
		{
			vector2f position;
			float angle;
		};
	}
}

#endif