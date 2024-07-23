#include "dsemi/math/quaternion.h"
#include <cmath>

namespace
{
    constexpr float c_pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062;
}

namespace dsemi {
	// Default unit quaternion
	Quaternion::Quaternion()
		:w(1.0f), x(0.0f), y(0.0f), z(0.0f)
	{
	}

	Quaternion::Quaternion(const vector3f& n, float angle)
	{
		// Convert from degrees to radians
		angle = angle / 360 * c_pi * 2;

		w = std::cos(angle / 2);
		float s = std::sin(angle / 2);
		x = n.x * s;
		z = n.y * s;
		y = n.z * s;
	}

	const Quaternion Quaternion::Inverse() const
	{
		Quaternion qOut;
		qOut.w = w;
		qOut.x = -x;
		qOut.y = -y;
		qOut.z = -z;
		return qOut;
	}
}
