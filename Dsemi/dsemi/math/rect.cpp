#include "dsemi/Math/rect.h"
#include <limits>

namespace dsemi {

	rect::rect(float x, float y, float width, float height)
		: x(x), y(y), width(width), height(height)
	{
		if (width - x <= std::numeric_limits<float>::epsilon())
		{
			width = 0.0f;
		}
		if (height - y <= std::numeric_limits<float>::epsilon())
		{
			height = 0.0f;
		}
	}

	rect::rect(Vector2f top_left, Vector2f bottom_right)
	{
		x = top_left.x;
		y = top_left.y;
		width  = bottom_right.x - x;
		height = bottom_right.y - y;
	}

}
