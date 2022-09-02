#pragma once

#include "Dsemi/Math/vector3f.h"

namespace dsemi {
	class Quaternion
	{
	public:
		Quaternion();
		Quaternion(const vector3f& n, float angle);
		const Quaternion Inverse() const noexcept;
	public:
		float w, x, y, z;
	};
}