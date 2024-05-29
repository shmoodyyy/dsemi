#pragma once

#include "Dsemi/Math/vector3f.h"
#include "Dsemi/Math/vector2.h"

namespace dsemi {
	struct vertex
	{
		vertex(vector3f pos = { 0.0f, 0.0f, 0.0f }, vector3f norm = { 0.0f, 0.0f, 0.0f }, vector2f uv = { 0.0f, 0.0f })
			: position(pos), normal(norm), texCoord(uv)
		{}

		vector3f position;
		vector3f normal;
		vector2f texCoord;
	};

	struct vertex2d
	{
		vertex2d(vector2f pos = { 0.0f, 0.0f }, vector2f norm = { 0.0f, 0.0f }, vector2f uv = { 0.0f, 0.0f })
			: position(pos), normal(norm), texCoord(uv)
		{}

		vector2f position;
		vector2f normal;
		vector2f texCoord;
	};
}