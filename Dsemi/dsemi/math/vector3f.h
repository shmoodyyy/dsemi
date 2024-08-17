#pragma once

namespace dsemi {
	class Vector3f
	{
	public:
		Vector3f();
		Vector3f(float x, float y, float z);

		// Properties of vector
		float magnitude() const;

		// Mathematical functions
		Vector3f normalized() const;
		Vector3f& normalize();
		Vector3f cross(const Vector3f& other) const;
		float dot(const Vector3f& other) const;
		float angle_to(const Vector3f& other) const;
		float distance(const Vector3f& other) const;

		// Operators
		Vector3f& operator+=(const Vector3f& rhs);
		Vector3f& operator-=(const Vector3f& rhs);

		bool operator==(const Vector3f& rhs) const;
		bool operator!=(const Vector3f& rhs) const;

		static Vector3f Up() { return Vector3f(0.0f, 1.0f, 0.0f); }
		static Vector3f Down() { return Vector3f(0.0f, -1.0f, 0.0f); }
		static Vector3f Right() { return Vector3f(1.0f, 0.0f, 0.0f); }
		static Vector3f Left() { return Vector3f(-1.0f, 0.0f, 0.0f); }
		static Vector3f Forward() { return Vector3f(0.0f, 0.0f, 1.0f); }
		static Vector3f Backward() { return Vector3f(0.0f, 0.0f, -1.0f); }
	public:
		float x, y, z;
	};

	Vector3f operator+(const Vector3f& lhs, const Vector3f& rhs);
	Vector3f operator-(const Vector3f& lhs, const Vector3f& rhs);
	Vector3f operator*(const Vector3f& lhs, const Vector3f& rhs);
	Vector3f operator/(const Vector3f& lhs, const Vector3f& rhs);
	Vector3f operator*(const Vector3f& lhs, const float& rhs);
	Vector3f operator/(const Vector3f& lhs, const float& rhs);
}
