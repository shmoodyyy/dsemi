#ifndef MATH_vector2fLOAT_HEADER
#define MATH_vector2fLOAT_HEADER

namespace dsemi {
	class vector2f
	{
	public:
		vector2f();
		vector2f(float x, float y);

		// Properties of vector
		float magnitude() const noexcept;

		// Mathematical functions
		vector2f normalized() const noexcept;
		float dot(const vector2f& other) const noexcept;
		float angle_to(const vector2f& other) const noexcept;
		float distance(const vector2f& other) const noexcept;

		// Operators
		vector2f& operator+=(const vector2f& rhs) noexcept;
		vector2f& operator-=(const vector2f& rhs) noexcept;

		bool operator==(const vector2f& rhs) const noexcept;
		bool operator!=(const vector2f& rhs) const noexcept;
	public:
		float x, y;
	};


	vector2f operator+(const vector2f& lhs, const vector2f& rhs) noexcept;
	vector2f operator-(const vector2f& lhs, const vector2f& rhs) noexcept;
	vector2f operator*(const vector2f& lhs, const vector2f& rhs) noexcept;
	vector2f operator*(const vector2f& lhs, const float& rhs) noexcept;
	vector2f operator/(const vector2f& lhs, const float& rhs) noexcept;
}

#endif