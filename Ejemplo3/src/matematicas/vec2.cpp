#include "vec2.h"

#include <sstream>

vec2::vec2()
	: x(0.0f), y(0.0f)
{
}

vec2::vec2(float scalar)
	: x(scalar), y(scalar)
{
}

vec2::vec2(float x, float y)
	: x(x), y(y)
{
}

vec2::vec2(const vec3& vector)
{
	this->x = vector.x;
	this->y = vector.y;
}

vec2& vec2::Add(const vec2& other)
{
	x += other.x;
	y += other.y;

	return *this;
}

vec2& vec2::Subtract(const vec2& other)
{
	x -= other.x;
	y -= other.y;

	return *this;
}

vec2& vec2::Multiply(const vec2& other)
{
	x *= other.x;
	y *= other.y;

	return *this;
}

vec2& vec2::Divide(const vec2& other)
{
	x /= other.x;
	y /= other.y;

	return *this;
}

vec2& vec2::Add(float value)
{
	x += value;
	y += value;

	return *this;
}

vec2& vec2::Subtract(float value)
{
	x -= value;
	y -= value;

	return *this;
}

vec2& vec2::Multiply(float value)
{
	x *= value;
	y *= value;

	return *this;
}

vec2& vec2::Divide(float value)
{
	x /= value;
	y /= value;

	return *this;
}

vec2 operator+(vec2 left, const vec2& right)
{
	return left.Add(right);
}

vec2 operator-(vec2 left, const vec2& right)
{
	return left.Subtract(right);
}

vec2 operator*(vec2 left, const vec2& right)
{
	return left.Multiply(right);
}

vec2 operator/(vec2 left, const vec2& right)
{
	return left.Divide(right);
}

vec2 operator+(vec2 left, float value)
{
	return vec2(left.x + value, left.y + value);
}

vec2 operator-(vec2 left, float value)
{
	return vec2(left.x - value, left.y - value);
}

vec2 operator*(vec2 left, float value)
{
	return vec2(left.x * value, left.y * value);
}

vec2 operator/(vec2 left, float value)
{
	return vec2(left.x / value, left.y / value);
}

vec2& vec2::operator+=(const vec2& other)
{
	return Add(other);
}

vec2& vec2::operator-=(const vec2& other)
{
	return Subtract(other);
}

vec2& vec2::operator*=(const vec2& other)
{
	return Multiply(other);
}

vec2& vec2::operator/=(const vec2& other)
{
	return Divide(other);
}

vec2& vec2::operator+=(float value)
{
	return Add(value);
}

vec2& vec2::operator-=(float value)
{
	return Subtract(value);
}

vec2& vec2::operator*=(float value)
{
	return Multiply(value);
}

vec2& vec2::operator/=(float value)
{
	return Divide(value);
}

bool vec2::operator==(const vec2& other) const
{
	return x == other.x && y == other.y;
}

bool vec2::operator!=(const vec2& other) const
{
	return !(*this == other);
}

bool vec2::operator<(const vec2& other) const
{
	return x < other.x && y < other.y;
}

bool vec2::operator<=(const vec2& other) const
{
	return x <= other.x && y <= other.y;
}

bool vec2::operator>(const vec2& other) const
{
	return x > other.x && y > other.y;
}

bool vec2::operator>=(const vec2& other) const
{
	return x >= other.x && y >= other.y;
}

float vec2::Distance(const vec2& other) const
{
	float a = x - other.x;
	float b = y - other.y;
	return sqrt(a * a + b * b);
}

float vec2::Dot(const vec2& other) const
{
	return x * other.x + y * other.y;
}

float vec2::Magnitude() const
{
	return sqrt(x * x + y * y);
}

vec2 vec2::Normalise() const
{
	float length = Magnitude();
	return vec2(x / length, y / length);
}

std::string vec2::ToString() const
{
	std::stringstream result;
	result << "vec2: (" << x << ", " << y << ")";
	return result.str();
}

std::ostream& operator<<(std::ostream& stream, const vec2& vector)
{
	stream << vector.ToString();
	return stream;
}
