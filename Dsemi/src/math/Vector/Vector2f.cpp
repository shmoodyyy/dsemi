#include "dspch.h"

#include "Dsemi/Math/Vector/vector2f.h"
#include "Dsemi/graphics/directx/dxinclude.h"

namespace dsemi {
	// Constructors
	vector2f::vector2f()
		:x(0.0f), y(0.0f)
	{
	}
	vector2f::vector2f(float x, float y)
		: x(x), y(y)
	{
	}


	// Properties
	float vector2f::magnitude() const noexcept
	{
		return sqrt(x * x + y * y);
	}


	// Mathematical Functions
	vector2f vector2f::normalized() const noexcept
	{
		vector2f out(*this);
		out = out / out.magnitude();
		return out;
	}

	float vector2f::dot(const vector2f& other) const noexcept
	{
		return (x * other.x + y * other.y);
	}

	float vector2f::angle_to(const vector2f& other) const noexcept
	{
		return acos(this->dot(other) / (this->magnitude() * other.magnitude())) / (M_PI * 2.0f) * 360.0f;
	}

	float vector2f::distance(const vector2f& other) const noexcept
	{
		// Essentially doing the same as usual pythagorean bullshit, but due to operator
		// implementation and magnitude function it can be simply written down like this.
		return (vector2f(*this) - other).magnitude();
	}


	// Operators
	vector2f operator+(const vector2f& lhs, const vector2f& rhs) noexcept
	{
		float x, y;
		x = lhs.x + rhs.x;
		y = lhs.y + rhs.y;
		return vector2f(x, y);
	}
	vector2f operator-(const vector2f& lhs, const vector2f& rhs) noexcept
	{
		float x, y;
		x = lhs.x - rhs.x;
		y = lhs.y - rhs.y;
		return vector2f(x, y);
	}
	vector2f operator*(const vector2f& lhs, const vector2f& rhs) noexcept
	{
		float x, y;
		x = lhs.x * rhs.x;
		y = lhs.y * rhs.y;
		return vector2f(x, y);
	}
	vector2f operator*(const vector2f& lhs, const float& rhs) noexcept
	{
		float x, y;
		x = lhs.x * rhs;
		y = lhs.y * rhs;
		return vector2f(x, y);
	}
	vector2f operator/(const vector2f& lhs, const float& rhs) noexcept
	{
		float x, y;
		x = lhs.x / rhs;
		y = lhs.y / rhs;
		return vector2f(x, y);
	}
	vector2f& vector2f::operator+=(const vector2f& rhs) noexcept
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	vector2f& vector2f::operator-=(const vector2f& rhs) noexcept
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	bool vector2f::operator==(const vector2f& rhs) const noexcept
	{
		return (x == rhs.x && y == rhs.y);
	}
	bool vector2f::operator!=(const vector2f& rhs) const noexcept
	{
		return !(*this == rhs);
	}
}