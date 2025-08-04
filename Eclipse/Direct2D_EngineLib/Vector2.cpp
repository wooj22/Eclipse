#include "Vector2.h"

// static member init
const Vector2 Vector2::zero = Vector2(0.0f, 0.0f);
const Vector2 Vector2::one = Vector2(1.0f, 1.0f);
const Vector2 Vector2::up = Vector2(0.0f, 1.0f);
const Vector2 Vector2::down = Vector2(0.0f, -1.0f);
const Vector2 Vector2::left = Vector2(-1.0f, 0.0f);
const Vector2 Vector2::right = Vector2(1.0f, 0.0f);

// static function
float Vector2::Distance(const Vector2& a, const Vector2& b)
{
    return (a - b).Magnitude();
}

float Vector2:: Cross(const Vector2& a, const Vector2& b)
{
    return a.x * b.y - a.y * b.x;
}

float Vector2::Dot(const Vector2& a, const Vector2& b)
{
    return a.x * b.x + a.y * b.y;
}

Vector2 Vector2::Lerp(const Vector2& a, const Vector2& b, float t)
{
    return a + (b - a) * t;
}

float Vector2::Angle(const Vector2& from, const Vector2& to)
{
    float dot = Dot(from.Normalized(), to.Normalized());
    dot = std::fmax(std::fmin(dot, 1.0f), -1.0f); // Clamp
    return std::acos(dot) * (180.0f / 3.14159265358979323846f); // rad -> deg
}

// functions
Vector2 Vector2::operator-() const
{
    return Vector2(-x, -y);
}

Vector2 Vector2::operator+() const
{
    return Vector2(+x, +y);
}

Vector2 Vector2::operator+(const Vector2& other) const
{
    return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const
{
    return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator*(float scalar) const
{
    return Vector2(x * scalar, y * scalar);
}

Vector2 Vector2::operator*(const Vector2& other) const
{
    return Vector2{ x * other.x, y * other.y };
}

Vector2 Vector2::operator/(float scalar) const
{
    return Vector2(x / scalar, y / scalar);
}

Vector2& Vector2::operator+=(const Vector2& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector2& Vector2::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2& Vector2::operator/=(float scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
}

bool Vector2::operator==(const Vector2& other) const
{
    return x == other.x && y == other.y;
}

bool Vector2::operator!=(const Vector2& other) const 
{
    return !(*this == other);
}

Vector2 Vector2::Normalized() const
{
    float len = Magnitude();
    if (len == 0.0f)
        return Vector2(0, 0);
    return Vector2(x / len, y / len);
}

float Vector2::Magnitude() const
{
    return std::sqrt(x * x + y * y);
}

float Vector2::SqrMagnitude() const
{
    return x * x + y * y;
}

float Vector2::Cross(const Vector2& other) const
{
    return x * other.y - y * other.x;
}

float Vector2::Dot(const Vector2& other) const
{
    return x * other.x + y * other.y;
}

bool Vector2::IsZero() const
{
    const float epsilon = 1e-6f;
    return fabs(x) < epsilon && fabs(y) < epsilon;
}

Vector2 Vector2::Abs() const
{
    return Vector2(std::abs(x), std::abs(y));
}