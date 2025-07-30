#pragma once
#include <cmath>

class Vector2
{
public:
    float x;
    float y;

    // construct
    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float x, float y) : x(x), y(y) {}

    // static constants (unity like)
    static const Vector2 zero;
    static const Vector2 one;
    static const Vector2 up;
    static const Vector2 down;
    static const Vector2 left;
    static const Vector2 right;

    // static function
    static float Distance(const Vector2& a, const Vector2& b);
    static float Cross(const Vector2& a, const Vector2& b);
    static float Dot(const Vector2& a, const Vector2& b);
    static Vector2 Lerp(const Vector2& a, const Vector2& b, float t);
    static float Angle(const Vector2& from, const Vector2& to);

    // operator overloading
    Vector2 operator-() const;
    Vector2 operator+() const;

    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator*(float scalar) const;
    Vector2 operator*(const Vector2& other) const;
    Vector2 operator/(float scalar) const;

    Vector2& operator+=(const Vector2& other);
    Vector2& operator-=(const Vector2& other);
    Vector2& operator*=(float scalar);
    Vector2& operator/=(float scalar);

    // function
    Vector2 Normalized() const;
    float Magnitude() const;
    float SqrMagnitude() const;
    float Cross(const Vector2& other) const;
    float Dot(const Vector2& other) const;
    bool IsZero() const;
    Vector2 Abs() const;
};
