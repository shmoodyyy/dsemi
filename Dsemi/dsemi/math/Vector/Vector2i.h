#ifndef MATH_vector2iNTEGER_HEADER
#define MATH_vector2iNTEGER_HEADER

namespace dsemi {
	class vector2i
	{
	public:
		vector2i();
		vector2i(int x, int y);

		// Properties of vector
		int magnitude() const noexcept;

		// Mathematical functions
		vector2i normalized() const noexcept;
		int dot(const vector2i& other) const noexcept;
		int angle_to(const vector2i& other) const noexcept;
		int distance(const vector2i& other) const noexcept;

		// Operators
		vector2i operator+(const vector2i& rhs) noexcept;
		vector2i operator-(const vector2i& rhs) noexcept;
		vector2i operator*(const vector2i& rhs) noexcept;
		vector2i operator*(const int& rhs) noexcept;
		vector2i operator/(const int& rhs) noexcept;
		vector2i& operator+=(const vector2i& rhs) noexcept;
		vector2i& operator-=(const vector2i& rhs) noexcept;

		bool operator==(const vector2i& rhs) const noexcept;
		bool operator!=(const vector2i& rhs) const noexcept;
	public:
		int x, y;
	};
}

#endif