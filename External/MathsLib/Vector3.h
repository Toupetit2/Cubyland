#define _USE_MATH_DEFINES
#pragma once
#include <math.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>

template<typename T>
struct Vector3
{
	// STATIC PROPERTIES //
	static const Vector3 down;
	static const Vector3 up;
	static const Vector3 left;
	static const Vector3 right;
	static const Vector3 back;
	static const Vector3 forward;
	static const Vector3 one;
	static const Vector3 zero;
	static const Vector3 negativeInfinity;
	static const Vector3 positiveInfinity;

	// PROPERTIES //
	T x;
	T y;
	T z;

	float magnitude();
	float sqrMagnitude();
	Vector3<T> normalized();

	// CONSTRUCTORS //
	Vector3(T xVal, T yVal, T zVal) : x(xVal), y(yVal), z(zVal) {};

	// PUBLIC METHODS //
	void Normalize();
	void Set(T newX, T newY, T newZ);
	std::string ToString(std::string format = "F2");

	// STATIC METHODS //

	static Vector3<T> Normalize(Vector3<T>& value);
	static float Angle(Vector3 from, Vector3 to);
	static Vector3 ClampMagnitude(Vector3 vector, float maxLength);
	static Vector3 Cross(Vector3 lhs, Vector3 rhs);
	static float Distance(Vector3 a, Vector3 b);
	static float Dot(Vector3 lhs, Vector3 rhs);
	static Vector3 Lerp(Vector3 a, Vector3 b, float t);
	static Vector3 LerpUnclamped(Vector3 a, Vector3 b, float t);
	static Vector3 Max(Vector3 lhs, Vector3 rhs);
	static Vector3 Min(Vector3 lhs, Vector3 rhs);
	static Vector3 MoveTowards(Vector3 current, Vector3 target, float maxDistanceDelta);
	static void OrthoNormalize(Vector3& normal, Vector3& tangent);
	static void OrthoNormalize(Vector3& normal, Vector3& tangent, Vector3& binormal);
	static Vector3 Project(Vector3 vector, Vector3 onNormal);
	static Vector3 ProjectOnPlane(Vector3 vector, Vector3 planeNormal);
	static Vector3 RotateTowards(Vector3 current, Vector3 target, float maxRadiansDelta, float maxMagnitudeDelta);
	static Vector3 Slerp(Vector3 a, Vector3 b, float t);
	static Vector3 SlerpUnclamped(Vector3 a, Vector3 b, float t);
	static Vector3 Reflect(Vector3 inDirection, Vector3 inNormal);
	static Vector3 Scale(Vector3 a, Vector3 b);
	void Scale(Vector3 scale);
	static float SignedAngle(Vector3 from, Vector3 to, Vector3 axis);

	// OPERATORS //
	Vector3 operator+(Vector3 other);
	Vector3 operator-(Vector3 other);
	Vector3 operator-();
	Vector3 operator*(float d);
	Vector3 operator*(Vector3 other);
	Vector3 operator/(float d);
	Vector3 operator/(Vector3 other);
	bool operator==(Vector3 other);
	T operator[](int index);
};

#include "Vector3.inl"