#include "Dsemi/Math/Vector/Vector2i.h"
#include "Dsemi/graphics/directx/dxinclude.h"
#include <cmath>

namespace
{
    constexpr float c_pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062;
}

namespace dsemi {
	// Constructors
	Vector2i::Vector2i()
		:x(0.0f), y(0.0f)
	{
	}
	Vector2i::Vector2i(int x, int y)
		: x(x), y(y)
	{
	}


	// Properties
	int Vector2i::magnitude() const
	{
		return sqrt(x * x + y * y);
	}


	// Mathematical Functions
	Vector2i Vector2i::normalized() const
	{
		Vector2i out(*this);
		out = out / out.magnitude();
		return out;
	}

	int Vector2i::dot(const Vector2i& other) const
	{
		return (x * other.x + y * other.y);
	}

	int Vector2i::angle_to(const Vector2i& other) const
	{
		return std::acos(this->dot(other) / (this->magnitude() * other.magnitude())) / (c_pi * 2.0f) * 360.0f;
	}

	int Vector2i::distance(const Vector2i& other) const
	{
		// Essentially doing the same as usual pythagorean bullshit, but due to operator
		// implementation and magnitude function it can be simply written down like this.
		return (Vector2i(*this) - other).magnitude();
	}


	// Operators
	Vector2i Vector2i::operator+(const Vector2i& rhs)
	{
		int x, y;
		x = this->x + rhs.x;
		y = this->y + rhs.y;
		return Vector2i(x, y);
	}
	Vector2i Vector2i::operator-(const Vector2i& rhs)
	{
		int x, y;
		x = this->x - rhs.x;
		y = this->y - rhs.y;
		return Vector2i(x, y);
	}
	Vector2i Vector2i::operator*(const Vector2i& rhs)
	{
		int x, y;
		x = this->x * rhs.x;
		y = this->y * rhs.y;
		return Vector2i(x, y);
	}
	Vector2i Vector2i::operator*(const int& rhs)
	{
		int x, y;
		x = this->x * rhs;
		y = this->y * rhs;
		return Vector2i(x, y);
	}
	Vector2i Vector2i::operator/(const int& rhs)
	{
		int x, y;
		x = this->x / rhs;
		y = this->y / rhs;
		return Vector2i(x, y);
	}
	Vector2i& Vector2i::operator+=(const Vector2i& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Vector2i& Vector2i::operator-=(const Vector2i& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	bool Vector2i::operator==(const Vector2i& rhs) const
	{
		return (x == rhs.x && y == rhs.y);
	}
	bool Vector2i::operator!=(const Vector2i& rhs) const
	{
		return !(*this == rhs);
	}
}
