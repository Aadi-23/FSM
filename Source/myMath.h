#pragma once
// ############################
// #     Vector Operators     #
// ############################
#include "raylib.h"

struct Vector2i
{
	int x = 0;
	int y = 0;
};

constexpr inline Vector2 operator+=(Vector2& a, const Vector2& b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

constexpr inline Vector2i operator+=(Vector2i& a, const Vector2i& b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

constexpr inline Vector2 operator-=(Vector2& a, const Vector2& b)
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

constexpr inline Vector2i operator-=(Vector2i& a, const Vector2i& b)
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

constexpr inline Vector2 operator*=(Vector2& a, const Vector2& b)
{
	a.x *= b.x;
	a.y *= b.y;
	return a;
}

constexpr inline Vector2i operator*=(Vector2i& a, const Vector2i& b)
{
	a.x *= b.x;
	a.y *= b.y;
	return a;
}

constexpr inline Vector2 operator*=(Vector2& a, float b)
{
	a.x *= b;
	a.y *= b;
	return a;
}

constexpr inline Vector2i operator*=(Vector2i& a, int b)
{
	a.x *= b;
	a.y *= b;
	return a;
}

constexpr inline Vector2 operator/=(Vector2& a, const Vector2& b)
{
	a.x /= b.x;
	a.y /= b.y;
	return a;
}

constexpr inline Vector2i operator/=(Vector2i& a, const Vector2i& b)
{
	a.x /= b.x;
	a.y /= b.y;
	return a;
}

constexpr inline Vector2 operator/=(Vector2& a, float b)
{
	a.x /= b;
	a.y /= b;
	return a;
}

constexpr inline Vector2i operator/=(Vector2i& a, int b)
{
	a.x /= b;
	a.y /= b;
	return a;
}

constexpr inline Vector2 operator+(Vector2 a, const Vector2& b)
{
	a += b;
	return a;
}

constexpr inline Vector2i operator+(Vector2i a, const Vector2i& b)
{
	a += b;
	return a;
}

constexpr inline Vector2 operator-(Vector2 a, const Vector2& b)
{
	a -= b;
	return a;
}

constexpr inline Vector2i operator-(Vector2i a, const Vector2i& b)
{
	a -= b;
	return a;
}

constexpr inline Vector2 operator*(Vector2 a, const Vector2& b)
{
	a *= b;
	return a;
}

constexpr inline Vector2i operator*(Vector2i a, const Vector2i& b)
{
	a *= b;
	return a;
}

constexpr inline Vector2 operator*(Vector2 a, float b)
{
	a *= b;
	return a;
}

constexpr inline Vector2i operator*(Vector2i a, int b)
{
	a *= b;
	return a;
}

constexpr inline Vector2 operator/(Vector2 a, const Vector2& b)
{
	a /= b;
	return a;
}

constexpr inline Vector2i operator/(Vector2i a, const Vector2i& b)
{
	a /= b;
	return a;
}

constexpr inline Vector2 operator/(Vector2 a, int b)
{
	a /= (float)b;
	return a;
}

constexpr inline bool operator==(Vector2i a, Vector2i b)
	{
	if ((a.x == b.x) && (a.y == b.y)) { return true; }
	else { return false; }
	}