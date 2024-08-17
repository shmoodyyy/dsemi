#pragma once

#include "Dsemi/Math/Vector3f.h"
#include "Dsemi/Math/vector2.h"

namespace dsemi {
	struct vertex
	{
		vertex(Vector3f pos = { 0.0f, 0.0f, 0.0f }, Vector3f norm = { 0.0f, 0.0f, 0.0f }, Vector2f uv = { 0.0f, 0.0f })
			: position(pos), normal(norm), texCoord(uv)
		{}

		Vector3f position;
		Vector3f normal;
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
