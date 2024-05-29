#ifndef MATH_VECTOR2DOUBLE_HEADER
#define MATH_VECTOR2DOUBLE_HEADER

namespace dsemi {
	class vector2d
	{
	public:
		vector2d();
		vector2d(double x, double y);

		// Properties of vector
		double magnitude() const noexcept;

		// Mathematical functions
		vector2d normalized() const noexcept;
		double dot(const vector2d& other) const noexcept;
		double angle_to(const vector2d& other) const noexcept;
		double distance(const vector2d& other) const noexcept;

		// Operators
		vector2d operator+(const vector2d& rhs) noexcept;
		vector2d operator-(const vector2d& rhs) noexcept;
		vector2d operator*(const vector2d& rhs) noexcept;
		vector2d operator*(const double& rhs) noexcept;
		vector2d operator/(const double& rhs) noexcept;
		vector2d& operator+=(const vector2d& rhs) noexcept;
		vector2d& operator-=(const vector2d& rhs) noexcept;

		bool operator==(const vector2d& rhs) const noexcept;
		bool operator!=(const vector2d& rhs) const noexcept;
	public:
		double x, y;
	};
}

#endif