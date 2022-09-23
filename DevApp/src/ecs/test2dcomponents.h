#ifndef HEADER_DEVAPP_ECS_COMPONENTS_TEST2DCOMPONENTS
#define HEADER_DEVAPP_ECS_COMPONENTS_TEST2DCOMPONENTS

#include <dsemi/ecs/component.h>
#include <dsemi/math/Vector2.h>
#include <dsemi/graphics/color.h>
#include <vector>

struct transform2d : public dsemi::ecs::icomponent<transform2d>
{
	transform2d(dsemi::vector2f pos = dsemi::vector2f(0.0f, 0.0f), float ang = 0.0f, dsemi::vector2f scale = dsemi::vector2f(0.0f, 0.0f))
		: position(pos), rotation(ang), scale(scale)
	{}
	dsemi::vector2f position;
	float rotation;
	dsemi::vector2f scale;
};

struct sprite_render : public dsemi::ecs::icomponent<sprite_render>
{
	sprite_render()
	{}
	dsemi::vector2i dimension;
	dsemi::color32 albedo;
};

void render_sprites(dsemi::ecs::world& entities)
{

}

#endif