#ifndef MATH_VECTOR2DOUBLE_HEADER
#define MATH_VECTOR2DOUBLE_HEADER

namespace dsemi {
	class vector2d
	{
	public:
		vector2d();
		vector2d(double x, double y);

		// Properties of vector
		double magnitude() const;

		// Mathematical functions
		vector2d normalized() const;
		double dot(const vector2d& other) const;
		double angle_to(const vector2d& other) const;
		double distance(const vector2d& other) const;

		// Operators
		vector2d operator+(const vector2d& rhs);
		vector2d operator-(const vector2d& rhs);
		vector2d operator*(const vector2d& rhs);
		vector2d operator*(const double& rhs);
		vector2d operator/(const double& rhs);
		vector2d& operator+=(const vector2d& rhs);
		vector2d& operator-=(const vector2d& rhs);

		bool operator==(const vector2d& rhs) const;
		bool operator!=(const vector2d& rhs) const;
	public:
		double x, y;
	};
}

#endif