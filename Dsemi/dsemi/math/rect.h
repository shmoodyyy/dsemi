#ifndef DSEMI_MATH_RECT_H
#define DSEMI_MATH_RECT_H

#include "dsemi/Math/Vector2.h"

namespace dsemi {

	struct rect
	{
		rect(float x, float y, float width, float height);
		rect(vector2f top_left, vector2f bottom_right);

		const vector2f top_left() const noexcept { return vector2f(x, y); }
		const vector2f bottom_right() const noexcept { return vector2f(x + width, y + height); }

		float x;
		float y;
		float width;
		float height;
	};
}

#endif