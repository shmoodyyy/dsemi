#include "Dsemi/Math/Vector/vector2d.h"
#include "Dsemi/graphics/directx/dxinclude.h"
#include <cmath>

namespace
{
    constexpr float c_pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062;
}

namespace dsemi {
	// Constructors
	vector2d::vector2d()
		:x(0.0f), y(0.0f)
	{
	}
	vector2d::vector2d(double x, double y)
		: x(x), y(y)
	{
	}


	// Properties
	double vector2d::magnitude() const noexcept
	{
		return sqrt(x * x + y * y);
	}


	// Mathematical Functions
	vector2d vector2d::normalized() const noexcept
	{
		vector2d out(*this);
		out = out / out.magnitude();
		return out;
	}

	double vector2d::dot(const vector2d& other) const noexcept
	{
		return (x * other.x + y * other.y);
	}

	double vector2d::angle_to(const vector2d& other) const noexcept
	{
		return std::acos(this->dot(other) / (this->magnitude() * other.magnitude())) / (c_pi * 2.0f) * 360.0f;
	}

	double vector2d::distance(const vector2d& other) const noexcept
	{
		// Essentially doing the same as usual pythagorean bullshit, but due to operator
		// implementation and magnitude function it can be simply written down like this.
		return (vector2d(*this) - other).magnitude();
	}


	// Operators
	vector2d vector2d::operator+(const vector2d& rhs) noexcept
	{
		double x, y;
		x = this->x + rhs.x;
		y = this->y + rhs.y;
		return vector2d(x, y);
	}
	vector2d vector2d::operator-(const vector2d& rhs) noexcept
	{
		double x, y;
		x = this->x - rhs.x;
		y = this->y - rhs.y;
		return vector2d(x, y);
	}
	vector2d vector2d::operator*(const vector2d& rhs) noexcept
	{
		double x, y;
		x = this->x * rhs.x;
		y = this->y * rhs.y;
		return vector2d(x, y);
	}
	vector2d vector2d::operator*(const double& rhs) noexcept
	{
		double x, y;
		x = this->x * rhs;
		y = this->y * rhs;
		return vector2d(x, y);
	}
	vector2d vector2d::operator/(const double& rhs) noexcept
	{
		double x, y;
		x = this->x / rhs;
		y = this->y / rhs;
		return vector2d(x, y);
	}
	vector2d& vector2d::operator+=(const vector2d& rhs) noexcept
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	vector2d& vector2d::operator-=(const vector2d& rhs) noexcept
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	bool vector2d::operator==(const vector2d& rhs) const noexcept
	{
		return (x == rhs.x && y == rhs.y);
	}
	bool vector2d::operator!=(const vector2d& rhs) const noexcept
	{
		return !(*this == rhs);
	}
}
