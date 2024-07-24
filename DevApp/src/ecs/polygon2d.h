#pragma once

#include "Dsemi/ecs/Component.h"
#include "Dsemi/graphics/graphics.h"
#include <vector>

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

void render_polygon2d(polygon2d* poly_component, dsemi::graphics::Device* gfx);
