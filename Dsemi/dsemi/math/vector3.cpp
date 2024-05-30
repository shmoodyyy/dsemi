#include "dsemi/math/vector3f.h"
#include "dsemi/graphics/directx/dxinclude.h"
#include <cmath>

namespace
{
    constexpr float c_pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062;
}

namespace dsemi {
	// Constructors
	vector3f::vector3f()
		:x(0.0f), y(0.0f), z(0.0f)
	{
	}
	vector3f::vector3f(float x, float y, float z)
		:x(x), y(y), z(z)
	{
	}


	// Properties
	float vector3f::magnitude() const noexcept
	{
		return sqrt(x*x + y*y + z*z);
	}


	// Mathematical Functions
	vector3f vector3f::normalized() const noexcept
	{
		vector3f out(*this);
		out = out / out.magnitude();
		return out;
	}

	vector3f& vector3f::normalize() noexcept
	{
		*this = this->normalized();
		return *this;
	}

	vector3f vector3f::cross(const vector3f & other) const noexcept
	{
		vector3f cross;

		cross.x = y * other.z - z * other.y;
		cross.y = z * other.x - x * other.z;
		cross.z = x * other.y - y * other.x;

		return cross;
	}

	float vector3f::dot(const vector3f& other) const noexcept
	{
		return (x * other.x + y * other.y + z * other.z);
	}

	float vector3f::angle_to(const vector3f& other) const noexcept
	{
		return std::acos( this->dot(other) / (this->magnitude() * other.magnitude()) ) / (c_pi * 2.0f) * 360.0f;
	}

	float vector3f::distance(const vector3f& other) const noexcept
	{
		// Essentially doing the same as usual pythagorean bullshit, but due to operator
		// implementation and magnitude function it can be simply written down like this.
		return (vector3f(*this) - other).magnitude();
	}


	// Operators

	// Vector-Vector arithmetics

	vector3f operator+(const vector3f& lhs, const vector3f& rhs) noexcept
	{
		float x, y, z;
		x = lhs.x + rhs.x;
		y = lhs.y + rhs.y;
		z = lhs.z + rhs.z;
		return vector3f(x, y, z);
	}
	vector3f operator-(const vector3f& lhs, const vector3f& rhs) noexcept
	{
		float x, y, z;
		x = lhs.x - rhs.x;
		y = lhs.y - rhs.y;
		z = lhs.z - rhs.z;
		return vector3f(x, y, z);
	}
	vector3f operator*(const vector3f& lhs, const vector3f& rhs) noexcept
	{
		float x, y, z;
		x = lhs.x * rhs.x;
		y = lhs.y * rhs.y;
		z = lhs.z * rhs.z;
		return vector3f(x,y,z);
	}
	vector3f operator/(const vector3f& lhs, const vector3f& rhs) noexcept
	{
		float x, y, z;
		x = lhs.x / rhs.x;
		y = lhs.y / rhs.y;
		z = lhs.z / rhs.z;
		return vector3f(x, y, z);
	}

	// Vector-Float arithmetics

	vector3f operator*(const vector3f& lhs, const float& rhs) noexcept
	{
		float x, y, z;
		x = lhs.x * rhs;
		y = lhs.y * rhs;
		z = lhs.z * rhs;
		return vector3f(x, y, z);
	}
	vector3f operator/(const vector3f& lhs, const float& rhs) noexcept
	{
		float x, y, z;
		x = lhs.x / rhs;
		y = lhs.y / rhs;
		z = lhs.z / rhs;
		return vector3f(x, y, z);
	}
	vector3f& vector3f::operator+=(const vector3f& rhs) noexcept
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	vector3f& vector3f::operator-=(const vector3f& rhs) noexcept
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	bool vector3f::operator==(const vector3f& rhs) const noexcept
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z);
	}
	bool vector3f::operator!=(const vector3f& rhs) const noexcept
	{
		return !(*this == rhs);
	}
}
