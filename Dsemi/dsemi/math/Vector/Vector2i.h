#ifndef MATH_vector2iNTEGER_HEADER
#define MATH_vector2iNTEGER_HEADER

namespace dsemi {
	class vector2i
	{
	public:
		vector2i();
		vector2i(int x, int y);

		// Properties of vector
		int magnitude() const;

		// Mathematical functions
		vector2i normalized() const;
		int dot(const vector2i& other) const;
		int angle_to(const vector2i& other) const;
		int distance(const vector2i& other) const;

		// Operators
		vector2i operator+(const vector2i& rhs);
		vector2i operator-(const vector2i& rhs);
		vector2i operator*(const vector2i& rhs);
		vector2i operator*(const int& rhs);
		vector2i operator/(const int& rhs);
		vector2i& operator+=(const vector2i& rhs);
		vector2i& operator-=(const vector2i& rhs);

		bool operator==(const vector2i& rhs) const;
		bool operator!=(const vector2i& rhs) const;
	public:
		int x, y;
	};
}

#endif