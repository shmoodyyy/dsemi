#ifndef DSEMI_MATH_RECT_H
#define DSEMI_MATH_RECT_H
#include "dsemi/Math/Vector2.h"

namespace dsemi {

	struct rect
	{
		rect(float x, float y, float width, float height);
		rect(Vector2f top_left, Vector2f bottom_right);

		const Vector2f top_left() const { return Vector2f(x, y); }
		const Vector2f bottom_right() const { return Vector2f(x + width, y + height); }

		float x;
		float y;
		float width;
		float height;
	};
}

#endif
