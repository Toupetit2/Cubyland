#pragma once
#include <math.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <string>
#include "Quaternion.h"
#include "Vector4.h"


template<typename T>
struct Matrix4x4
{
	static const Matrix4x4 zero;
	static const Matrix4x4 identity;
	// PROPERTIES //

	T m[4][4];
	T determinant;

	// CONSTRUCTORS //
	Matrix4x4(Vector4<T> col0, Vector4<T> col1, Vector4<T> col2, Vector4<T> col3)
	{
		for (int i = 0; i < 4; ++i)
		{
			m[i][0] = col0[i];
			m[i][1] = col1[i];
			m[i][2] = col2[i];
			m[i][3] = col3[i];
		}
	}

	Matrix4x4()
	{
		for (int col = 0; col < 4; ++col)
		{
			for (int row = 0; row < 4; ++row)
			{
				m[col][row] = (T)0;
			}
		}
		m[0][0] = (T)1;
		m[1][1] = (T)1;
		m[2][2] = (T)1;
		m[3][3] = (T)1;
	}

	// PUBLIC METHODS //
	Matrix4x4<T> Inverse();
	bool IsIdentity();
	Vector3<T> LossyScale();
	Quaternion<T> Rotation();
	Matrix4x4<T> Transpose();
	
	Vector4<T> GetColumn(int index);
	Vector4<T> GetRow(int index);
	Vector3<T> GetPosition();
	Vector3<T> MultiplyPoint(Vector3<T> point);
	Vector3<T> MultiplyVector(Vector3<T> vector);
	void SetColumn(int index, Vector4<T> column);
	void SetRow(int index, Vector4<T> row);
	void SetTRS(Vector3<T> pos, Quaternion<T> q, Vector3<T> s);
	std::string ToString(std::string format = "F2");
	bool ValidTRS();

	// STATIC METHODS //
	static Matrix4x4<T> Frustum(float left, float right, float bottom, float top, float zNear, float zFar);
	static bool Inverse3DAffine(Matrix4x4<T> input, Matrix4x4<T>& result);
	static Matrix4x4<T> LookAt(Vector3<T> from, Vector3<T> to, Vector3<T> up);
	static Matrix4x4<T> Ortho(float left, float right, float bottom, float top, float zNear, float zFar);
	static Matrix4x4<T> Perspective(float fov, float aspect, float zNear, float zFar);
	static Matrix4x4<T> Rotate(Quaternion<T> q);
	static Matrix4x4<T> Scale(Vector3<T> vector);
	static Matrix4x4<T> Translate(Vector3<T> vector);
	static Matrix4x4<T> TRS(Vector3<T> pos, Quaternion<T> q, Vector3<T> s);


	// OPERATORS //
	Matrix4x4<T> operator*(Matrix4x4<T> rhs);
	Vector4<T> operator*(Vector4<T> v);
	bool operator==(const Matrix4x4<T>& rhs) const;
};

#include "Matrix4x4.inl"