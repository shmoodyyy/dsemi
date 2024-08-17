#pragma once

#include "Dsemi/Math/Vector3f.h"

namespace dsemi {
	class Quaternion
	{
	public:
		Quaternion();
		Quaternion(const Vector3f& n, float angle);
		const Quaternion Inverse() const;
	public:
		float w, x, y, z;
	};
}