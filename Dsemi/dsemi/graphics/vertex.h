#pragma once

#include "Dsemi/Math/vector3f.h"
#include "Dsemi/Math/vector2.h"

namespace dsemi {
	struct vertex
	{
		vertex(vector3f pos = { 0.0f, 0.0f, 0.0f }, vector3f norm = { 0.0f, 0.0f, 0.0f }, Vector2f uv = { 0.0f, 0.0f })
			: position(pos), normal(norm), texCoord(uv)
		{}

		vector3f position;
		vector3f normal;
		Vector2f texCoord;
	};

	struct vertex2d
	{
		vertex2d(Vector2f pos = { 0.0f, 0.0f }, Vector2f norm = { 0.0f, 0.0f }, Vector2f uv = { 0.0f, 0.0f })
			: position(pos), normal(norm), texCoord(uv)
		{}

		Vector2f position;
		Vector2f normal;
		Vector2f texCoord;
	};
}
