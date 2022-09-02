#include "dspch.h"

#include "Dsemi/Math/Vector/vector2i.h"
#include "Dsemi/graphics/directx/dxinclude.h"

namespace dsemi {
	// Constructors
	vector2i::vector2i()
		:x(0.0f), y(0.0f)
	{
	}
	vector2i::vector2i(int x, int y)
		: x(x), y(y)
	{
	}


	// Properties
	int vector2i::magnitude() const noexcept
	{
		return sqrt(x * x + y * y);
	}


	// Mathematical Functions
	vector2i vector2i::normalized() const noexcept
	{
		vector2i out(*this);
		out = out / out.magnitude();
		return out;
	}

	int vector2i::dot(const vector2i& other) const noexcept
	{
		return (x * other.x + y * other.y);
	}

	int vector2i::angle_to(const vector2i& other) const noexcept
	{
		return acos(this->dot(other) / (this->magnitude() * other.magnitude())) / (M_PI * 2.0f) * 360.0f;
	}

	int vector2i::distance(const vector2i& other) const noexcept
	{
		// Essentially doing the same as usual pythagorean bullshit, but due to operator
		// implementation and magnitude function it can be simply written down like this.
		return (vector2i(*this) - other).magnitude();
	}


	// Operators
	vector2i vector2i::operator+(const vector2i& rhs) noexcept
	{
		int x, y;
		x = this->x + rhs.x;
		y = this->y + rhs.y;
		return vector2i(x, y);
	}
	vector2i vector2i::operator-(const vector2i& rhs) noexcept
	{
		int x, y;
		x = this->x - rhs.x;
		y = this->y - rhs.y;
		return vector2i(x, y);
	}
	vector2i vector2i::operator*(const vector2i& rhs) noexcept
	{
		int x, y;
		x = this->x * rhs.x;
		y = this->y * rhs.y;
		return vector2i(x, y);
	}
	vector2i vector2i::operator*(const int& rhs) noexcept
	{
		int x, y;
		x = this->x * rhs;
		y = this->y * rhs;
		return vector2i(x, y);
	}
	vector2i vector2i::operator/(const int& rhs) noexcept
	{
		int x, y;
		x = this->x / rhs;
		y = this->y / rhs;
		return vector2i(x, y);
	}
	vector2i& vector2i::operator+=(const vector2i& rhs) noexcept
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	vector2i& vector2i::operator-=(const vector2i& rhs) noexcept
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	bool vector2i::operator==(const vector2i& rhs) const noexcept
	{
		return (x == rhs.x && y == rhs.y);
	}
	bool vector2i::operator!=(const vector2i& rhs) const noexcept
	{
		return !(*this == rhs);
	}
}