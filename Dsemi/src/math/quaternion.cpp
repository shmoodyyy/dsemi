#include "dspch.h"

#include "dsemi/math/quaternion.h"

namespace dsemi {
	// Default unit quaternion
	Quaternion::Quaternion()
		:w(1.0f), x(0.0f), y(0.0f), z(0.0f)
	{
	}

	Quaternion::Quaternion(const vector3f& n, float angle)
	{
		// Convert from degrees to radians
		angle = angle / 360 * (float)M_PI * 2;

		w = cos(angle / 2);
		float s = sin(angle / 2);
		x = n.x * s;
		z = n.y * s;
		y = n.z * s;
	}

	const Quaternion Quaternion::Inverse() const noexcept
	{
		Quaternion qOut;
		qOut.w = w;
		qOut.x = -x;
		qOut.y = -y;
		qOut.z = -z;
		return qOut;
	}
}