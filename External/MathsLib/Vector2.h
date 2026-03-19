#define _USE_MATH_DEFINES
#pragma once
#include <math.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>

template<typename T>
struct Vector2
{
	// STATIC PROPERTIES //
	static const Vector2 down;
	static const Vector2 up;
	static const Vector2 left;
	static const Vector2 right;
	static const Vector2 one;
	static const Vector2 zero;
	static const Vector2 negativeInfinity;
	static const Vector2 positiveInfinity;

	// PROPERTIES //
	T x;
	T y;

	float magnitude();
	float sqrMagnitude();
	Vector2 normalized();

	// CONSTRUCTORS //
	Vector2(T xVal, T yVal) : x(xVal), y(yVal) {};

	// PUBLIC METHODS //
	void Normalize();
	void Set(T newX, T newY);
	std::string ToString(std::string format = "F2");

	// STATIC METHODS //
	static float Angle(Vector2 from, Vector2 to);
	static Vector2 ClampMagnitude(Vector2 vector, float maxLength);
	static float Distance(Vector2 a, Vector2 b);
	static float Dot(Vector2 lhs, Vector2 rhs);
	static Vector2 Lerp(Vector2 a, Vector2 b, float t);
	static Vector2 LerpUnclamped(Vector2 a, Vector2 b, float t);
	static Vector2 Max(Vector2 lhs, Vector2 rhs);
	static Vector2 Min(Vector2 lhs, Vector2 rhs);
	static Vector2 MoveTowards(Vector2 current, Vector2 target, float maxDistanceDelta);
	static Vector2 Perpendicular(Vector2 inDirection);
	static Vector2 Reflect(Vector2 inDirection, Vector2 inNormal);
	static Vector2 Scale(Vector2 a, Vector2 b);
	void Scale(Vector2 scale);
	static float SignedAngle(Vector2 from, Vector2 to);

	// OPERATORS //
	Vector2 operator+(Vector2 other);
	Vector2 operator-(Vector2 other);
	Vector2 operator-();
	Vector2 operator*(float d);
	Vector2 operator*(Vector2 other);
	Vector2 operator/(float d);
	Vector2 operator/(Vector2 other);
	bool operator==(Vector2 other);
	T operator[](int index);
};

#include "Vector2.inl"