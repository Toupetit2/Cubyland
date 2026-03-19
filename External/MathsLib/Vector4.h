#define _USE_MATH_DEFINES
#pragma once
#include <string>
#include "Vector2.h"
#include "Vector3.h"

template<typename T>
struct Vector4
{
	// STATIC PROPERTIES //
	static const Vector4 one;
	static const Vector4 zero;
	static const Vector4 negativeInfinity;
	static const Vector4 positiveInfinity;

	// PROPERTIES //
	T x;
	T y;
	T z;
	T w;

	float magnitude();
	float sqrMagnitude();
	Vector4<T> normalized();

	// CONSTRUCTORS //
	Vector4(T xVal = 0, T yVal = 0, T zVal = 0, T wVal = 0) : x(xVal), y(yVal), z(zVal), w(wVal) {};
	Vector4(const Vector3<T>& v3, T wVal = 0) : x(v3.x), y(v3.y), z(v3.z), w(wVal) {}
	Vector4(const Vector2<T>& v2, T zVal = 0, T wVal = 0) : x(v2.x), y(v2.y), z(zVal), w(wVal) {}

	// PUBLIC METHODS //
	void Normalize();
	void Set(T newX, T newY, T newZ, T newW);
	std::string ToString(std::string format = "F2");

	// STATIC METHODS //

	static Vector4<T> Normalize(Vector4<T>& value);
	static float Dot(Vector4<T> lhs, Vector4<T> rhs);
	static Vector4<T> Lerp(Vector4<T> a, Vector4<T> b, float t);
	static Vector4<T> LerpUnclamped(Vector4<T> a, Vector4<T> b, float t);
	static Vector4<T> Max(Vector4<T> lhs, Vector4<T> rhs);
	static Vector4<T> Min(Vector4<T> lhs, Vector4<T> rhs);
	static Vector4<T> MoveTowards(Vector4<T> current, Vector4<T> target, float maxDistanceDelta);
	static Vector4<T> Project(Vector4<T> a, Vector4<T> b);
	static float Distance(Vector4<T> a, Vector4<T> b);

	// OPERATORS //
	Vector4<T> operator+(Vector4<T> other);
	Vector4<T> operator-(Vector4<T> other);
	Vector4<T> operator-();
	Vector4<T> operator*(float d);
	Vector4<T> operator*(Vector4<T> other);
	Vector4<T> operator/(float d);
	Vector4<T> operator/(Vector4<T> other);
	bool operator==(Vector4<T> other);
	T operator[](int index);

	explicit operator Vector3<T>() const
	{
		return Vector3<T>(x, y, z);
	}

	explicit operator Vector2<T>() const
	{
		return Vector2<T>(x, y);
	}
};

#include "Vector4.inl"