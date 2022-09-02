#pragma once

namespace dsemi {
	class vector3f
	{
	public:
		vector3f();
		vector3f(float x, float y, float z);

		// Properties of vector
		float magnitude() const noexcept;

		// Mathematical functions
		vector3f normalized() const noexcept;
		vector3f& normalize() noexcept;
		vector3f cross(const vector3f& other) const noexcept;
		float dot(const vector3f& other) const noexcept;
		float angle_to(const vector3f& other) const noexcept;
		float distance(const vector3f& other) const noexcept;

		// Operators
		vector3f& operator+=(const vector3f& rhs) noexcept;
		vector3f& operator-=(const vector3f& rhs) noexcept;

		bool operator==(const vector3f& rhs) const noexcept;
		bool operator!=(const vector3f& rhs) const noexcept;

		static vector3f Up() { return vector3f(0.0f, 1.0f, 0.0f); }
		static vector3f Down() { return vector3f(0.0f, -1.0f, 0.0f); }
		static vector3f Right() { return vector3f(1.0f, 0.0f, 0.0f); }
		static vector3f Left() { return vector3f(-1.0f, 0.0f, 0.0f); }
		static vector3f Forward() { return vector3f(0.0f, 0.0f, 1.0f); }
		static vector3f Backward() { return vector3f(0.0f, 0.0f, -1.0f); }
	public:
		float x, y, z;
	};

	vector3f operator+(const vector3f& lhs, const vector3f& rhs) noexcept;
	vector3f operator-(const vector3f& lhs, const vector3f& rhs) noexcept;
	vector3f operator*(const vector3f& lhs, const vector3f& rhs) noexcept;
	vector3f operator/(const vector3f& lhs, const vector3f& rhs) noexcept;
	vector3f operator*(const vector3f& lhs, const float& rhs) noexcept;
	vector3f operator/(const vector3f& lhs, const float& rhs) noexcept;
}