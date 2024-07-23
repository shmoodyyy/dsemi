#pragma once

namespace dsemi {
	class vector3f
	{
	public:
		vector3f();
		vector3f(float x, float y, float z);

		// Properties of vector
		float magnitude() const;

		// Mathematical functions
		vector3f normalized() const;
		vector3f& normalize();
		vector3f cross(const vector3f& other) const;
		float dot(const vector3f& other) const;
		float angle_to(const vector3f& other) const;
		float distance(const vector3f& other) const;

		// Operators
		vector3f& operator+=(const vector3f& rhs);
		vector3f& operator-=(const vector3f& rhs);

		bool operator==(const vector3f& rhs) const;
		bool operator!=(const vector3f& rhs) const;

		static vector3f Up() { return vector3f(0.0f, 1.0f, 0.0f); }
		static vector3f Down() { return vector3f(0.0f, -1.0f, 0.0f); }
		static vector3f Right() { return vector3f(1.0f, 0.0f, 0.0f); }
		static vector3f Left() { return vector3f(-1.0f, 0.0f, 0.0f); }
		static vector3f Forward() { return vector3f(0.0f, 0.0f, 1.0f); }
		static vector3f Backward() { return vector3f(0.0f, 0.0f, -1.0f); }
	public:
		float x, y, z;
	};

	vector3f operator+(const vector3f& lhs, const vector3f& rhs);
	vector3f operator-(const vector3f& lhs, const vector3f& rhs);
	vector3f operator*(const vector3f& lhs, const vector3f& rhs);
	vector3f operator/(const vector3f& lhs, const vector3f& rhs);
	vector3f operator*(const vector3f& lhs, const float& rhs);
	vector3f operator/(const vector3f& lhs, const float& rhs);
}