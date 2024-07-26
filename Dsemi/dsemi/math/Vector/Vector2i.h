#ifndef MATH_vector2iNTEGER_HEADER
#define MATH_vector2iNTEGER_HEADER

namespace dsemi {
	class Vector2i
	{
	public:
		Vector2i();
		Vector2i(int x, int y);

		// Properties of vector
		int magnitude() const;

		// Mathematical functions
		Vector2i normalized() const;
		int dot(const Vector2i& other) const;
		int angle_to(const Vector2i& other) const;
		int distance(const Vector2i& other) const;

		// Operators
		Vector2i operator+(const Vector2i& rhs);
		Vector2i operator-(const Vector2i& rhs);
		Vector2i operator*(const Vector2i& rhs);
		Vector2i operator*(const int& rhs);
		Vector2i operator/(const int& rhs);
		Vector2i& operator+=(const Vector2i& rhs);
		Vector2i& operator-=(const Vector2i& rhs);

		bool operator==(const Vector2i& rhs) const;
		bool operator!=(const Vector2i& rhs) const;
	public:
		int x, y;
	};
}

#endif
