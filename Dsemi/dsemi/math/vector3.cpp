#include "dsemi/math/Vector3f.h"
#include "dsemi/graphics/directx/dxinclude.h"
#include <cmath>

namespace
{
    constexpr float c_pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062;
}

namespace dsemi {
	// Constructors
	Vector3f::Vector3f()
		:x(0.0f), y(0.0f), z(0.0f)
	{
	}
	Vector3f::Vector3f(float x, float y, float z)
		:x(x), y(y), z(z)
	{
	}


	// Properties
	float Vector3f::magnitude() const
	{
		return sqrt(x*x + y*y + z*z);
	}


	// Mathematical Functions
	Vector3f Vector3f::normalized() const
	{
		Vector3f out(*this);
		out = out / out.magnitude();
		return out;
	}

	Vector3f& Vector3f::normalize()
	{
		*this = this->normalized();
		return *this;
	}

	Vector3f Vector3f::cross(const Vector3f & other) const
	{
		Vector3f cross;

		cross.x = y * other.z - z * other.y;
		cross.y = z * other.x - x * other.z;
		cross.z = x * other.y - y * other.x;

		return cross;
	}

	float Vector3f::dot(const Vector3f& other) const
	{
		return (x * other.x + y * other.y + z * other.z);
	}

	float Vector3f::angle_to(const Vector3f& other) const
	{
		return std::acos( this->dot(other) / (this->magnitude() * other.magnitude()) ) / (c_pi * 2.0f) * 360.0f;
	}

	float Vector3f::distance(const Vector3f& other) const
	{
		// Essentially doing the same as usual pythagorean bullshit, but due to operator
		// implementation and magnitude function it can be simply written down like this.
		return (Vector3f(*this) - other).magnitude();
	}


	// Operators

	// Vector-Vector arithmetics

	Vector3f operator+(const Vector3f& lhs, const Vector3f& rhs)
	{
		float x, y, z;
		x = lhs.x + rhs.x;
		y = lhs.y + rhs.y;
		z = lhs.z + rhs.z;
		return Vector3f(x, y, z);
	}
	Vector3f operator-(const Vector3f& lhs, const Vector3f& rhs)
	{
		float x, y, z;
		x = lhs.x - rhs.x;
		y = lhs.y - rhs.y;
		z = lhs.z - rhs.z;
		return Vector3f(x, y, z);
	}
	Vector3f operator*(const Vector3f& lhs, const Vector3f& rhs)
	{
		float x, y, z;
		x = lhs.x * rhs.x;
		y = lhs.y * rhs.y;
		z = lhs.z * rhs.z;
		return Vector3f(x,y,z);
	}
	Vector3f operator/(const Vector3f& lhs, const Vector3f& rhs)
	{
		float x, y, z;
		x = lhs.x / rhs.x;
		y = lhs.y / rhs.y;
		z = lhs.z / rhs.z;
		return Vector3f(x, y, z);
	}

	// Vector-Float arithmetics

	Vector3f operator*(const Vector3f& lhs, const float& rhs)
	{
		float x, y, z;
		x = lhs.x * rhs;
		y = lhs.y * rhs;
		z = lhs.z * rhs;
		return Vector3f(x, y, z);
	}
	Vector3f operator/(const Vector3f& lhs, const float& rhs)
	{
		float x, y, z;
		x = lhs.x / rhs;
		y = lhs.y / rhs;
		z = lhs.z / rhs;
		return Vector3f(x, y, z);
	}
	Vector3f& Vector3f::operator+=(const Vector3f& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	Vector3f& Vector3f::operator-=(const Vector3f& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	bool Vector3f::operator==(const Vector3f& rhs) const
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z);
	}
	bool Vector3f::operator!=(const Vector3f& rhs) const
	{
		return !(*this == rhs);
	}
}
