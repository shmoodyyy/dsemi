#ifndef MATH_vector2fLOAT_HEADER
#define MATH_vector2fLOAT_HEADER

namespace dsemi {
	class vector2f
	{
	public:
		vector2f();
		vector2f(float x, float y);

		// Properties of vector
		float magnitude() const;

		// Mathematical functions
		vector2f normalized() const;
		float dot(const vector2f& other) const;
		float angle_to(const vector2f& other) const;
		float distance(const vector2f& other) const;

		// Operators
		vector2f& operator+=(const vector2f& rhs);
		vector2f& operator-=(const vector2f& rhs);

		bool operator==(const vector2f& rhs) const;
		bool operator!=(const vector2f& rhs) const;
	public:
		float x, y;
	};


	vector2f operator+(const vector2f& lhs, const vector2f& rhs);
	vector2f operator-(const vector2f& lhs, const vector2f& rhs);
	vector2f operator*(const vector2f& lhs, const vector2f& rhs);
	vector2f operator*(const vector2f& lhs, const float& rhs);
	vector2f operator/(const vector2f& lhs, const float& rhs);
}

#endif