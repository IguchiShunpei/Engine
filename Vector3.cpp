// ===================
// Vector3 Œ^
// ===================
#include "Vector3.h"

Vector3::Vector3()
	:x(0), y(0), z(0)
{
}
Vector3::Vector3(float x, float y, float z)
	: x(x), y(y), z(z)
{
}
float Vector3::length() const
{
	return std::sqrt(x * x + y * y + z * z);
}
Vector3& Vector3::normalize()
{
	float len = length();
	if (len != 0)
	{
		return*this /= len;
	}
	return*this;
}
float Vector3::dot(const Vector3& v, const Vector3& v2)
{
	return v.x * v2.x + v.y * v2.y + v.z * v2.z;
}
Vector3 Vector3::cross(const Vector3& v)const
{
	Vector3 temp;
	temp.x = this->y * v.z - this->z * v.y;
	temp.y = this->z * v.x - this->x * v.z;
	temp.z = this->x * v.y - this->y * v.x;
	return temp;
}

const Vector3 Vector3::lerp(const Vector3& start, const Vector3& end, const float t)
{
	return start * (1.0f - t) + end * t;
}

Vector3 Vector3::operator+()const
{
	return *this;
}
Vector3 Vector3::operator-()const
{
	return Vector3(-x, -y, -z);
}
Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}
Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}
Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}
Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}
// Vector3 ƒNƒ‰ƒX‚É‘®‚³‚È‚¢ŠÖ”ŒQ
// “ñ€‰‰Zq
const Vector3 operator+ (const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}
const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}
const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp *= s;
}
const Vector3 operator*(float s, const Vector3& v)
{
	return v * s;
}
const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp /= s;
}

