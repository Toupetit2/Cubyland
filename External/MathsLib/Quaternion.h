#define _USE_MATH_DEFINES
#pragma once
#include <math.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include "Vector3.h"

template<typename T>
struct Quaternion
{
	// PROPERTIES //
	T x;
	T y;
	T z;
	T w;

	Quaternion normalized();
	//euler angle

	

	// CONSTRUCTORS //
	Quaternion(T xVal = 0.f, T yVal = 0.f, T zVal = 0.f, T wVal = 1.f) : x(xVal), y(yVal), z(zVal), w(wVal) {};

	// PUBLIC METHODS //
	void Set(T newX, T newY, T newZ, T newW);
	void SetLookRotation(Vector3<T> view, Vector3<T> up = Vector3<T>::up);
	void ToAngleAxis(float& angle, Vector3<T>& axis = Vector3<T>::right);
	void SetFromToRotation(Vector3<T> fromDirection, Vector3<T> toDiretion);
	std::string ToString(std::string format = "F2");

	// STATIC METHODS //
	static Quaternion<T> Identity();
	static Quaternion Normalize(Quaternion q);
	static float Dot(Quaternion a, Quaternion b);
	static T Angle(Quaternion a, Quaternion b);
	static Quaternion AngleAxis(float angle, Vector3<T> axis);
	static Quaternion Lerp(Quaternion a, Quaternion b, float t);
	static Quaternion LerpUnclamped(Quaternion a, Quaternion b, float t);
	static Quaternion Slerp(Quaternion a, Quaternion b, float t);
	static Quaternion SlerpUnclamped(Quaternion a, Quaternion b, float t);
	static Quaternion FromToRotation(Vector3<T> fromDirection, Vector3<T> toDirection);
	static Quaternion LookRotation(Vector3<T> forward, Vector3<T> upwards = Vector3<T>::up);
	static Quaternion Inverse(Quaternion rotation);
	static Quaternion RotateTowards(Quaternion from, Quaternion to, float maxDegreesDelta);
	static Quaternion Euler(float x, float y, float z);

	// OPERATORS //
	Vector3<T> operator*(Vector3<T> point);
	Quaternion operator*(Quaternion other);
	bool operator==(Quaternion other);
	T operator[](int index);
};

#include "Quaternion.inl"