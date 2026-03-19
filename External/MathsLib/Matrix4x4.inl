#include "Matrix4x4.h"

template<typename T>
const Matrix4x4<T> Matrix4x4<T>::zero = Matrix4x4<T>(Vector4<T>(0, 0, 0, 0), Vector4<T>(0, 0, 0, 0), Vector4<T>(0, 0, 0, 0), Vector4<T>(0, 0, 0, 0));
template<typename T>
const Matrix4x4<T> Matrix4x4<T>::identity = Matrix4x4<T>(Vector4<T>(1, 0, 0, 0), Vector4<T>(0, 1, 0, 0), Vector4<T>(0, 0, 1, 0), Vector4<T>(0, 0, 0, 1));

template<typename T>
Matrix4x4<T> Matrix4x4<T>::Inverse()
{
	if (std::abs(determinant) < 0.0001f)
	{
		return zero;
	}

	Matrix4x4<T> result = Transpose();

	Vector3<T> translation(m[0][3], m[1][3], m[2][3]);

	Vector3<T> translationInv( -(result.m[0][0] * translation.x + result.m[0][1] * translation.y + result.m[0][2] * translation.z), -(result.m[1][0] * translation.x + result.m[1][1] * translation.y + result.m[1][2] * translation.z), -(result.m[2][0] * translation.x + result.m[2][1] * translation.y + result.m[2][2] * translation.z));

	result.m[0][3] = translationInv.x;
	result.m[1][3] = translationInv.y;
	result.m[2][3] = translationInv.z;

	return result;
}

template<typename T>
bool Matrix4x4<T>::IsIdentity()
{
	return *this == Matrix4x4<T>::identity;
}

template<typename T>
Vector3<T> Matrix4x4<T>::LossyScale()
{
	Vector3<T> scale;

	scale.x = std::sqrt(m[0][0] * m[0][0] + m[1][0] * m[1][0] + m[2][0] * m[2][0]);
	scale.y = std::sqrt(m[0][1] * m[0][1] + m[1][1] * m[1][1] + m[2][1] * m[2][1]);
	scale.z = std::sqrt(m[0][2] * m[0][2] + m[1][2] * m[1][2] + m[2][2] * m[2][2]);

	return scale;
}
template<typename T>
Quaternion<T> Matrix4x4<T>::Rotation()
{
	Vector3<T> scale = LossyScale();

	// Normaliser chaque colonne (pour retirer le scale)
	T r00 = m[0][0] / scale.x;
	T r01 = m[0][1] / scale.y;
	T r02 = m[0][2] / scale.z;
	T r10 = m[1][0] / scale.x;
	T r11 = m[1][1] / scale.y;
	T r12 = m[1][2] / scale.z;
	T r20 = m[2][0] / scale.x;
	T r21 = m[2][1] / scale.y;
	T r22 = m[2][2] / scale.z;

	Quaternion<T> q;

	T trace = r00 + r11 + r22;
	if (trace > 0)
	{
		T s = std::sqrt(trace + 1.0) * 2;
		q.w = 0.25 * s;
		q.x = (r21 - r12) / s;
		q.y = (r02 - r20) / s;
		q.z = (r10 - r01) / s;
	}
	else if (r00 > r11 && r00 > r22)
	{
		T s = std::sqrt(1.0 + r00 - r11 - r22) * 2;
		q.w = (r21 - r12) / s;
		q.x = 0.25 * s;
		q.y = (r01 + r10) / s;
		q.z = (r02 + r20) / s;
	}
	else if (r11 > r22)
	{
		T s = std::sqrt(1.0 + r11 - r00 - r22) * 2;
		q.w = (r02 - r20) / s;
		q.x = (r01 + r10) / s;
		q.y = 0.25 * s;
		q.z = (r12 + r21) / s;
	}
	else
	{
		T s = std::sqrt(1.0 + r22 - r00 - r11) * 2;
		q.w = (r10 - r01) / s;
		q.x = (r02 + r20) / s;
		q.y = (r12 + r21) / s;
		q.z = 0.25 * s;
	}

	return q;
}


template<typename T>
Matrix4x4<T> Matrix4x4<T>::Transpose()
{
	Matrix4x4<T> result;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			result.m[i][j] = m[j][i];
		}
	}

	return result;
}

template<typename T>
Vector4<T> Matrix4x4<T>::GetRow(int index)
{
	return Vector4<T>(m[index][0], m[index][1], m[index][2], m[index][3]);
}
template<typename T>
Vector4<T> Matrix4x4<T>::GetColumn(int index)
{
	return Vector4<T>(m[0][index], m[1][index], m[2][index], m[3][index]);
}

template<typename T>
Vector3<T> Matrix4x4<T>::GetPosition()
{
	Vector4<T> column(GetColumn(3));

	return Vector3<T>(column[0], column[1], column[2]);
}

template<typename T>
void Matrix4x4<T>::SetColumn(int index, Vector4<T> column)
{
	for (int j = 0; j < 4; j++)
	{
		m[index][j] = column[j];
	}
}

template<typename T>
void Matrix4x4<T>::SetRow(int index, Vector4<T> row) 
{
	for (int j = 0; j < 4; j++)
	{
		m[j][index] = row[j];
	}
}

template<typename T>
void Matrix4x4<T>::SetTRS(Vector3<T> pos, Quaternion<T> q, Vector3<T> s)
{
	SetColumn(0, Vector4<T>((1 - 2 * (q.y * q.y + q.z * q.z)) * s.x, (2 * (q.x * q.y + q.w * q.z)) * s.x, (2 * (q.x * q.z - q.w * q.y)) * s.x, 0));
	SetColumn(1, Vector4<T>((2 * (q.x * q.y - q.w * q.z)) * s.y, (1 - 2 * (q.x * q.x + q.z * q.z)) * s.y, (2 * (q.y * q.z + q.w * q.x)) * s.y, 0));
	SetColumn(2, Vector4<T>((2 * (q.x * q.z + q.w * q.y)) * s.z, (2 * (q.y * q.z - q.w * q.x)) * s.z, (1 - 2 * (q.x * q.x + q.y * q.y)) * s.z, 0));
	SetColumn(3, Vector4<T>(pos, 1));
}

template<typename T>
bool Matrix4x4<T>::ValidTRS()
{
	return m[3][0] == 0 && m[3][1] == 0 && m[3][2] == 0 && m[3][3] == 1 &&
		!(std::abs(m[0][0]) < 0.0001f && std::abs(m[0][1]) < 0.0001f && std::abs(m[0][2]) < 0.0001f) &&
		!(std::abs(m[1][0]) < 0.0001f && std::abs(m[1][1]) < 0.0001f && std::abs(m[1][2]) < 0.0001f) &&
		!(std::abs(m[2][0]) < 0.0001f && std::abs(m[2][1]) < 0.0001f && std::abs(m[2][2]) < 0.0001f);
}

template<typename T>
Vector3<T> Matrix4x4<T>::MultiplyPoint(Vector3<T> point)
{
	T x = m[0][0] * point.x + m[0][1] * point.y + m[0][2] * point.z + m[0][3];
	T y = m[1][0] * point.x + m[1][1] * point.y + m[1][2] * point.z + m[1][3];
	T z = m[2][0] * point.x + m[2][1] * point.y + m[2][2] * point.z + m[2][3];

	return Vector3<T>(x, y, z);
}

template<typename T>
Vector3<T> Matrix4x4<T>::MultiplyVector(Vector3<T> vector)
{
	T x = m[0][0] * vector.x + m[0][1] * vector.y + m[0][2] * vector.z;
	T y = m[1][0] * vector.x + m[1][1] * vector.y + m[1][2] * vector.z;
	T z = m[2][0] * vector.x + m[2][1] * vector.y + m[2][2] * vector.z;

	return Vector3<T>(x, y, z);
}


template<typename T>
std::string Matrix4x4<T>::ToString(std::string format)
{
	std::stringstream ss;

	if (format[0] == 'G') // General | The more compact of either fixed-point or scientific notation.
	{
		if (format.size() > 1) { // Si precision inscrite en parametre
			
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					if ((log10(m[i][j]) < -4 || log10(m[i][j]) > std::stoi(format.substr(1))))
					{
						format = "E" + format.substr(1);
						break;
					}
					else {
						format = "F" + format.substr(1);
					}
				}
			}

		}
		else { // If no format parameter
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					if ((log10(m[i][j]) < -4 || log10(m[i][j]) > 6))
					{
						format = "E" + format.substr(1);
						break;
					}
					else {
						format = "F" + format.substr(1);
					}
				}
			}
		}
	}

	if (format[0] == 'F') // Fixed-point | Integral and decimal digits with optional negative sign.
	{
		if (format.size() > 1) {
			ss << "[" << std::fixed << std::setprecision(std::stoi(format.substr(1))) << m[0][0] << ", " << m[1][0] << ", " << m[2][0] << ", " << m[3][0] << "]\n[" 
				<< m[0][1] << ", " << m[1][1] << ", " << m[2][1] << ", " << m[3][1] << "]\n["
				<< m[0][2] << ", " << m[1][2] << ", " << m[2][2] << ", " << m[3][2] << "]\n["
				<< m[0][3] << ", " << m[1][3] << ", " << m[2][3] << ", " << m[3][3] << "]";
		}
		else {
			ss << "[" << std::fixed << std::setprecision(2) << m[0][0] << ", " << m[1][0] << ", " << m[2][0] << ", " << m[3][0] << "]\n["
				<< m[0][1] << ", " << m[1][1] << ", " << m[2][1] << ", " << m[3][1] << "]\n["
				<< m[0][2] << ", " << m[1][2] << ", " << m[2][2] << ", " << m[3][2] << "]\n["
				<< m[0][3] << ", " << m[1][3] << ", " << m[2][3] << ", " << m[3][3] << "]";
		}
	}

	else if (format[0] == 'E') // Exponential (scientific) | Exponential notation.
	{
		if (format.size() > 1) {
			ss << "[" << std::scientific << std::setprecision(std::stoi(format.substr(1))) << m[0][0] << ", " << m[1][0] << ", " << m[2][0] << ", " << m[3][0] << "]\n["
				<< m[0][1] << ", " << m[1][1] << ", " << m[2][1] << ", " << m[3][1] << "]\n["
				<< m[0][2] << ", " << m[1][2] << ", " << m[2][2] << ", " << m[3][2] << "]\n["
				<< m[0][3] << ", " << m[1][3] << ", " << m[2][3] << ", " << m[3][3] << "]";
		}
		else {
			ss << "[" << std::fixed << std::setprecision(6) << m[0][0] << ", " << m[1][0] << ", " << m[2][0] << ", " << m[3][0] << "]\n["
				<< m[0][1] << ", " << m[1][1] << ", " << m[2][1] << ", " << m[3][1] << "]\n["
				<< m[0][2] << ", " << m[1][2] << ", " << m[2][2] << ", " << m[3][2] << "]\n["
				<< m[0][3] << ", " << m[1][3] << ", " << m[2][3] << ", " << m[3][3] << "]";
		}
	}

	return ss.str();
}


template<typename T>
Matrix4x4<T> Matrix4x4<T>::Frustum(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix4x4<T> m;

	const T x = (2 * zNear) / (right - left);
	const T y = (2 * zNear) / (top - bottom);
	const T a = (right + left) / (right - left);
	const T b = (top + bottom) / (top - bottom);
	const T c = -(zFar + zNear) / (zFar - zNear);
	const T d = -(2 * zFar * zNear) / (zFar - zNear);

	m.m[0][0] = x;   m.m[0][1] = 0;   m.m[0][2] = 0;    m.m[0][3] = 0;
	m.m[1][0] = 0;   m.m[1][1] = y;   m.m[1][2] = 0;    m.m[1][3] = 0;
	m.m[2][0] = a;   m.m[2][1] = b;   m.m[2][2] = c;    m.m[2][3] = d;
	m.m[3][0] = 0;   m.m[3][1] = 0;   m.m[3][2] = -1;   m.m[3][3] = 0;

	return m;
}


template<typename T>
bool Matrix4x4<T>::Inverse3DAffine(Matrix4x4<T> input, Matrix4x4<T>& result)
{
	// Vérifie que la dernière ligne correspond bien à une matrice 3D affine
	if (m[0][3] == 0 && m[1][3] == 0 && m[2][3] == 0 && m[3][3] == 1)
	{
		// ici on peut faire l’inversion "optimisée"
		Matrix4x4<T> result;

		// R = sous-matrice 3x3
		result.m[0][0] = m[0][0]; result.m[0][1] = m[1][0]; result.m[0][2] = m[2][0];
		result.m[1][0] = m[0][1]; result.m[1][1] = m[1][1]; result.m[1][2] = m[2][1];
		result.m[2][0] = m[0][2]; result.m[2][1] = m[1][2]; result.m[2][2] = m[2][2];
		result.m[3][3] = 1;

		// Translation
		Vector3<T> t(m[0][3], m[1][3], m[2][3]);
		Vector3<T> tInv( -(result.m[0][0] * t.x + result.m[0][1] * t.y + result.m[0][2] * t.z),	-(result.m[1][0] * t.x + result.m[1][1] * t.y + result.m[1][2] * t.z), -(result.m[2][0] * t.x + result.m[2][1] * t.y + result.m[2][2] * t.z));

		result.m[0][3] = tInv.x;
		result.m[1][3] = tInv.y;
		result.m[2][3] = tInv.z;
	}

	// Sinon, on retombe sur la version complète
	result = input.Inverse();
}

template<typename T>
Matrix4x4<T> Matrix4x4<T>::LookAt(Vector3<T> from, Vector3<T> to, Vector3<T> up)
{
	return Matrix4x4<T>::TRS(from, Quaternion<T>::LookRotation(to - from, up), Vector3<T>::one);
}

template<typename T>
Matrix4x4<T> Matrix4x4<T>::Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix4x4<T> m = Matrix4x4<T>::Identity();

	m.m[0][0] = 2 / (right - 1);
	m.m[1][1] = 2 / (top - bottom);
	m.m[2][2] = -2 / (zFar - zNear);

	m.m[0][3] = -(right + 1) / (right - 1);
	m.m[1][3] = -(top + bottom) / (top - bottom);
	m.m[2][3] = -(zFar + zNear) / (zFar - zNear);

	return m;
}

template<typename T>
Matrix4x4<T> Matrix4x4<T>::Scale(Vector3<T> vector)
{
	Matrix4x4<T> result = Matrix4x4<T>::identity;
	result.m[0][0] = vector.x;
	result.m[1][1] = vector.y;
	result.m[2][2] = vector.z;
	
	return result;
}

template<typename T>
Matrix4x4<T> Matrix4x4<T>::Translate(Vector3<T> vector)
{
	Matrix4x4<T> m;

	m.m[0][3] = vector.x;
	m.m[1][3] = vector.y;
	m.m[2][3] = vector.z;

	return m;
}

template<typename T>
Matrix4x4<T> Matrix4x4<T>::Rotate(Quaternion<T> q)
{
	return Matrix4x4<T>(Vector4<T>((1 - 2 * (q.y * q.y + q.z * q.z)), (2 * (q.x * q.y + q.w * q.z)), (2 * (q.x * q.z - q.w * q.y)), 0), Vector4<T>((2 * (q.x * q.y - q.w * q.z)), (1 - 2 * (q.x * q.x + q.z * q.z)), (2 * (q.y * q.z + q.w * q.x)), 0), Vector4<T>((2 * (q.x * q.z + q.w * q.y)), (2 * (q.y * q.z - q.w * q.x)), (1 - 2 * (q.x * q.x + q.y * q.y)), 0), Vector4<T>(0, 0, 0, 1));
}
template<typename T>
Matrix4x4<T> Matrix4x4<T>::Perspective(float fov, float aspect, float zNear, float zFar)
{
	Matrix4x4<T> m;

	float top = zNear * std::tan((fov * 180 / M_PI) / 2.f);
	float right = top * aspect;

	return Matrix4x4<T>::Frustum(-right, right, -top, top, zNear, zFar);
}

template<typename T>
Matrix4x4<T> Matrix4x4<T>::TRS(Vector3<T> pos, Quaternion<T> q, Vector3<T> s)
{
	return Matrix4x4<T>(
		Vector4<T>((1 - 2 * (q.y * q.y + q.z * q.z)) * s.x, (2 * (q.x * q.y + q.w * q.z)) * s.x, (2 * (q.x * q.z - q.w * q.y)) * s.x, 0),
		Vector4<T>((2 * (q.x * q.y - q.w * q.z)) * s.y, (1 - 2 * (q.x * q.x + q.z * q.z)) * s.y, (2 * (q.y * q.z + q.w * q.x)) * s.y, 0),
		Vector4<T>((2 * (q.x * q.z + q.w * q.y)) * s.z, (2 * (q.y * q.z - q.w * q.x)) * s.z, (1 - 2 * (q.x * q.x + q.y * q.y)) * s.z, 0),
		Vector4<T>(pos, 1));
}


template<typename T>
Matrix4x4<T> Matrix4x4<T>::operator*(Matrix4x4<T> other)
{
	Matrix4x4<T> result;

	for (int col = 0; col < 4; ++col)
	{
		for (int row = 0; row < 4; ++row)
		{
			T sum = 0;
			for (int k = 0; k < 4; ++k)
			{
				sum += m[k][row] * other.m[col][k];
			}
			result.m[col][row] = sum;
		}
	}

	return result;
}

template<typename T>
Vector4<T> Matrix4x4<T>::operator*(Vector4<T> v)
{
	Vector4<T> result;

	result.x = m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w;
	result.y = m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w;
	result.z = m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w;
	result.w = m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w;

	return result;
}
template<typename T>
bool Matrix4x4<T>::operator==(const Matrix4x4<T>& rhs) const
{
	for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (m[i][j] != rhs.m[i][j])
                return false;
        }
    }
    return true;
}