#ifndef MATH_VECTOR2FLOAT_HEADER
#define MATH_VECTOR2FLOAT_HEADER

namespace dsemi {
	class Vector2f
	{
	public:
		Vector2f();
		Vector2f(float x, float y);

		// Properties of vector
		float magnitude() const;

		// Mathematical functions
		Vector2f normalized() const;
		float dot(const Vector2f& other) const;
		float angle_to(const Vector2f& other) const;
		float distance(const Vector2f& other) const;

		// Operators
		Vector2f& operator+=(const Vector2f& rhs);
		Vector2f& operator-=(const Vector2f& rhs);

		bool operator==(const Vector2f& rhs) const;
		bool operator!=(const Vector2f& rhs) const;
	public:
		float x, y;
	};


	Vector2f operator+(const Vector2f& lhs, const Vector2f& rhs);
	Vector2f operator-(const Vector2f& lhs, const Vector2f& rhs);
	Vector2f operator*(const Vector2f& lhs, const Vector2f& rhs);
	Vector2f operator*(const Vector2f& lhs, const float& rhs);
	Vector2f operator/(const Vector2f& lhs, const float& rhs);
}

#endif
