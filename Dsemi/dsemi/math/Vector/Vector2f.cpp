#include "Dsemi/Math/Vector/Vector2f.h"
#include "Dsemi/graphics/directx/dxinclude.h"
#include <cmath>

namespace
{
    constexpr float c_pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062;
}

namespace dsemi {
	// Constructors
	Vector2f::Vector2f()
		:x(0.0f), y(0.0f)
	{
	}
	Vector2f::Vector2f(float x, float y)
		: x(x), y(y)
	{
	}


	// Properties
	float Vector2f::magnitude() const
	{
		return sqrt(x * x + y * y);
	}


	// Mathematical Functions
	Vector2f Vector2f::normalized() const
	{
		Vector2f out(*this);
		out = out / out.magnitude();
		return out;
	}

	float Vector2f::dot(const Vector2f& other) const
	{
		return (x * other.x + y * other.y);
	}

	float Vector2f::angle_to(const Vector2f& other) const
	{
		return std::acos(this->dot(other) / (this->magnitude() * other.magnitude())) / (c_pi * 2.0f) * 360.0f;
	}

	float Vector2f::distance(const Vector2f& other) const
	{
		// Essentially doing the same as usual pythagorean bullshit, but due to operator
		// implementation and magnitude function it can be simply written down like this.
		return (Vector2f(*this) - other).magnitude();
	}


	// Operators
	Vector2f operator+(const Vector2f& lhs, const Vector2f& rhs)
	{
		float x, y;
		x = lhs.x + rhs.x;
		y = lhs.y + rhs.y;
		return Vector2f(x, y);
	}
	Vector2f operator-(const Vector2f& lhs, const Vector2f& rhs)
	{
		float x, y;
		x = lhs.x - rhs.x;
		y = lhs.y - rhs.y;
		return Vector2f(x, y);
	}
	Vector2f operator*(const Vector2f& lhs, const Vector2f& rhs)
	{
		float x, y;
		x = lhs.x * rhs.x;
		y = lhs.y * rhs.y;
		return Vector2f(x, y);
	}
	Vector2f operator*(const Vector2f& lhs, const float& rhs)
	{
		float x, y;
		x = lhs.x * rhs;
		y = lhs.y * rhs;
		return Vector2f(x, y);
	}
	Vector2f operator/(const Vector2f& lhs, const float& rhs)
	{
		float x, y;
		x = lhs.x / rhs;
		y = lhs.y / rhs;
		return Vector2f(x, y);
	}
	Vector2f& Vector2f::operator+=(const Vector2f& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Vector2f& Vector2f::operator-=(const Vector2f& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	bool Vector2f::operator==(const Vector2f& rhs) const
	{
		return (x == rhs.x && y == rhs.y);
	}
	bool Vector2f::operator!=(const Vector2f& rhs) const
	{
		return !(*this == rhs);
	}
}
