#include "Quaternion.h"

// Returns this quaternion with a magnitude of 1.
template<typename T>
Quaternion<T> Quaternion<T>::normalized()
{
	float mag = std::sqrt(x * x + y * y + z * z + w * w);
	return Quaternion<T>(x / mag, y / mag, z / mag, w / mag);
}

// Converts this quaternion to a quaternion with the same orientation but with a magnitude of 1.0.
template<typename T>
Quaternion<T> Quaternion<T>::Normalize(Quaternion q)
{
	float mag = std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
	return Quaternion<T>(q.x / mag, q.y / mag, q.z / mag, q.w / mag);
}

// Set x, y, z and w components of an existing Quaternion.
template<typename T>
void Quaternion<T>::Set(T newX, T newY, T newZ, T newW)
{
	x = newX;
	y = newY;
	z = newZ;
	w = newW;
}

// The identity rotation (0, 0, 0, 1).
template<typename T>
Quaternion<T> Quaternion<T>::Identity()
{
	return Quaternion<T>(0.f, 0.f, 0.f, 1.f);
}

// Creates a rotation which rotates from fromDirection to toDirection.
template<typename T>
void Quaternion<T>::SetFromToRotation(Vector3<T> fromDirection, Vector3<T> toDirection)
{
	Vector3<T> f = fromDirection.normalized();
	Vector3<T> t = toDirection.normalized();

	float dot = Vector3<T>::Dot(f, t);
	Vector3<T> axis = Vector3<T>::Cross(f, t);

	if (std::abs(dot - 1) < 0.0001f)
	{
		return Quaternion<T>::Identity();
	}
	if (std::abs(dot + 1) < 0.0001f)
	{
		Vector3<T> b;
		if (std::abs(f.x ) > std::abs(f.z))
		{
			b(-f.y, f.x, 0);
		}
		else
		{
			b(0, -f.z, f.y);
		}
		Vector3<T>::OrthoNormalize(f, b);
		x = b.x;
		y = b.y;
		z = b.z;
		w = (T)0;
		
	}
	Vector3<T> v3 = axis / std::sqrt(1 + dot);

	x = v3.x;
	y = v3.y;
	z = v3.z;
	w = std::sqrt(1 + dot) * 2;
}

// Creates a rotation with the specified forward and upwards directions.
template<typename T>
void Quaternion<T>::SetLookRotation(Vector3<T> view, Vector3<T> up)
{
	Vector3<T> zV3 = view.normalized();
	Vector3<T> xV3 = Vector3<T>::Cross(up, z).normalized();
	Vector3<T> yV3 = Vector3<T>::Cross(z, x);

	T trace = xV3.x + yV3.y + zV3.z;
	Quaternion<T> q;

	if (trace > 0)
	{
		T s = std::sqrt(trace + 1) * 2;
		w = 0.25f * s;
		x = (yV3.z - zV3.y) / s;
		y = (zV3.x - xV3.z) / s;
		z = (xV3.y - yV3.x) / s;
	}
	else
	{
		q = Quaternion<T>::Identity();
	}

	q.normalized();
}

// The dot product between two rotations.
template<typename T>
float Quaternion<T>::Dot(Quaternion<T> a, Quaternion<T> b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// 	Returns the angle in degrees between two rotations a and b. The resulting angle ranges from 0 to 180.
template<typename T>
T Quaternion<T>::Angle(Quaternion<T> a, Quaternion<T> b)
{
	return 2 * std::acos(std::abs(Quaternion<T>::Dot(a, b))) * 180 / M_PI;
}

// 	Returns the angle in degrees between two rotations a and b. The resulting angle ranges from 0 to 180.
template<typename T>
Quaternion<T>  Quaternion<T>::AngleAxis(float angle, Vector3<T> axis)
{
	Vector3<T> axisNorm = axis.normalized();
	float angleRadians = angle * (M_PI / 180.f);
	return Quaternion<T>(axisNorm.x * sin(angleRadians / 2), axisNorm.y * sin(angleRadians / 2), axisNorm.z * sin(angleRadians / 2), cos(angleRadians / 2));
}

// 	Interpolates between a and b by t and normalizes the result afterwards.
template<typename T>
Quaternion<T> Quaternion<T>::Lerp(Quaternion a, Quaternion b, float t)
{
	return LerpUnclamped(a, b, std::clamp(t, 0.f, 1.f));
}

// Interpolates between a and b by t and normalizes the result afterwards. The parameter t is not clamped.
template<typename T>
static Quaternion<T> Quaternion<T>::LerpUnclamped(Quaternion a, Quaternion b, float t)
{
	float dot = Quaternion<T>::Dot(a, b);

	Quaternion<T> bcopy = b;

	if (dot < 0.f)
	{
		bcopy.w = -bcopy.w;
		bcopy.x = -bcopy.x;
		bcopy.y = -bcopy.y;
		bcopy.z = -bcopy.z;
	}

	Quaternion<T> result;
	result.w = (1 - t) * a.w + t * bcopy.w;
	result.x = (1 - t) * a.x + t * bcopy.x;
	result.y = (1 - t) * a.y + t * bcopy.y;
	result.z = (1 - t) * a.z + t * bcopy.z;

	return result.normalized();
}

// Spherically linear interpolates between unit quaternions a and b by a ratio of t.
template<typename T>
Quaternion<T> Quaternion<T>::Slerp(Quaternion a, Quaternion b, float t)
{
	return SlerpUnclamped(a, b, std::clamp(t, 0.f, 1.f));
}

// Spherically linear interpolates between unit quaternions a and b by t.
template<typename T>
Quaternion<T> Quaternion<T>::SlerpUnclamped(Quaternion a, Quaternion b, float t)
{
	float dot = Quaternion<T>::Dot(a, b);

	Quaternion<T> bcopy = b;

	if (dot < 0.f)
	{
		bcopy.w = -bcopy.w;
		bcopy.x = -bcopy.x;
		bcopy.y = -bcopy.y;
		bcopy.z = -bcopy.z;
	}

	float theta = std::acos(Dot(a.normalized(), bcopy.normalized()));

	float w1 = std::sin((1 - t) * theta) / std::sin(theta);
	float w2 = std::sin(t * theta) / std::sin(theta);

	Quaternion<T> result;
	result.w = a.w * w1 + bcopy.w * w2;
	result.x = a.x * w1 + bcopy.x * w2;
	result.y = a.y * w1 + bcopy.y * w2;
	result.z = a.z * w1 + bcopy.z * w2;

	return result.normalized();
}

// Returns a formatted string for this quaternion.
template<typename T>
std::string Quaternion<T>::ToString(std::string format)
{
	std::stringstream ss;

	if (format[0] == 'G') // General | The more compact of either fixed-point or scientific notation.
	{
		if (format.size() > 1) { // Si precision inscrite en parametre
			if ((log10(x) < -4 || log10(x) > std::stoi(format.substr(1))) || (log10(y) < -4 || log10(y) > std::stoi(format.substr(1)) || (log10(z) < -4 || log10(z) > std::stoi(format.substr(1))) || (log10(w) < -4 || log10(w) > std::stoi(format.substr(1)))))
			{
				format = "E" + format.substr(1);
			}
			else {
				format = "F" + format.substr(1);
			}
		}
		else { // If no format parameter
			if ((log10(x) < -4 || log10(x) > 6 || (log10(y) < -4 || log10(y) > 6) || (log10(z) < -4 || log10(z) > 6) || (log10(w) < -4 || log10(w) > 6)))
			{
				format = "E" + format.substr(1);
			}
			else {
				format = "F" + format.substr(1);
			}
		}
	}

	if (format[0] == 'F') // Fixed-point | Integral and decimal digits with optional negative sign.
	{

		if (format.size() > 1) {
			ss << "(" << std::fixed << std::setprecision(std::stoi(format.substr(1))) << x << ", " << y << ", " << z << ", " << w << ")";
		}
		else {
			ss << "(" << std::fixed << std::setprecision(2) << x << ", " << y << ", " << z << ", " << w << ")";
		}
	}

	else if (format[0] == 'E') // Exponential (scientific) | Exponential notation.
	{
		if (format.size() > 1) {
			ss << "(" << std::scientific << std::setprecision(std::stoi(format.substr(1))) << x << ", " << y << ", " << z << ", " << w << ")";
		}
		else {
			ss << "(" << std::fixed << std::setprecision(6) << x << ", " << y << ", " << z << ", " << w << ")";
		}
	}

	return ss.str();
}

// Converts a rotation to angle-axis representation.
template<typename T>
void Quaternion<T>::ToAngleAxis(float& angle, Vector3<T>& axis)
{
	if (std::abs(std::sqrt(x * x + y * y + z * z + w * w)-1) > 0.0001f)
	{
		throw std::runtime_error("Quaternion need to be normalized before calling Quaternion<T>::ToNormalAxis()");
	}

	angle = 2 * std::acos(w) * (180.0 / M_PI);

	float s = std::sin(angle / 2);
	if (s < 0.00001f)
	{
		axis = Vector3<T>::right;
	}
	else
	{
		axis = Vector3<T>(x, y, z) / s;
	}
}

// Returns the Inverse of rotation.
template<typename T>
Quaternion<T> Quaternion<T>::Inverse(Quaternion<T> rotation)
{
	T normSq = rotation.x * rotation.x + rotation.y * rotation.y + rotation.z * rotation.z + rotation.w * rotation.w;
	return Quaternion<T>(-rotation.x / normSq, -rotation.y / normSq, -rotation.z / normSq, rotation.w / normSq);
}

// 	Rotates a rotation from towards to.
template<typename T>
Quaternion<T> Quaternion<T>::RotateTowards(Quaternion from, Quaternion to, float maxDegreesDelta)
{
	float angle = Dot(from, to);
	float maxRadiansDelta = maxDegreesDelta * (M_PI / 180.0f);


	if (angle < maxRadiansDelta)
	{
		return to;
	}

	float t = maxRadiansDelta / angle;

	return Slerp(from, to, t);
}

// Converts an input Euler angle rotation specified as three floats to a Quaternion.
template<typename T>
Quaternion<T> Quaternion<T>::Euler(float x, float y, float z)
{
	x *= (M_PI / 180.0f);
	y *= (M_PI / 180.0f);
	z *= (M_PI / 180.0f);

	Quaternion<T> xQ(std::sin(x / 2), 0, 0, std::cos(x / 2));
	Quaternion<T> yQ(0, std::sin(y / 2), 0, std::cos(y / 2));
	Quaternion<T> zQ(0, 0, std::sin(z / 2), std::cos(z / 2));

	return yQ * xQ * zQ;
}

// Creates a rotation from fromDirection to toDirection.
template<typename T>
Quaternion<T> Quaternion<T>::FromToRotation(Vector3<T> fromDirection, Vector3<T> toDirection)
{
	Vector3<T> fNorm = fromDirection.normalized();
	Vector3<T> tNorm = toDirection.normalized();

	Vector3<T> axis = fNorm * tNorm;

	float angle = std::acos(Dot(fNorm, tNorm));

	if (angle < 0.0001f)
	{
		return Quaternion<T>::Identity();
	}

	Vector3<T> result = axis * std::sin(angle / 2);

	return Quaternion<T>(result.x, result.y, result.z, std::cos(angle / 2));
}

// Creates a rotation with the specified forward and upwards directions.
template<typename T>
Quaternion<T> Quaternion<T>::LookRotation(Vector3<T> forward, Vector3<T> upwards)
{
	Vector3<T> z = forward.normalized();
	Vector3<T> x = Vector3<T>::Cross(upwards.normalized(), z).normalized();
	Vector3<T> y = Vector3<T>::Cross(z, x);

	T m00 = x.x;
	T m01 = y.x;
	T m02 = z.x;
	T m10 = x.y; 
	T m11 = y.y;
	T m12 = z.y;
	T m20 = x.z;
	T m21 = y.z;
	T m22 = z.z;

	T w = 0.5f * std::sqrt(1 + m00 + m11 + m22);

	return Quaternion<T>((m21 - m12) / (4 * w), (m02 - m20) / (4 * w), (m10 - m01) / (4 * w), w);
}

// Transform the Vector3 'point' using the quaternion.
template<typename T>
Vector3<T> Quaternion<T>::operator*(Vector3<T> point) // Multiplies a vector by a number.
{
	Quaternion<T> v3(point.x, point.y, point.z, 0);
	Quaternion<T> q = Quaternion::Normalize(Quaternion<T>(x, y, z, w));
	Quaternion<T> qInv(-q.x, -q.y, -q.z, q.w);

	Quaternion<T> result = q * v3 * qInv;

	return Vector3<T>(result.x, result.y, result.z);
}

// Combines rotations of the quaternion and the other.
template<typename T>
Quaternion<T> Quaternion<T>::operator*(Quaternion<T> other) // Multiplies a vector by another vector.
{
	return Quaternion<T>(
		w * other.x + x * other.w + y * other.z - z * other.y, // x
		w * other.y - x * other.z + y * other.w + z * other.x, // y
		w * other.z + x * other.y - y * other.x + z * other.w, // z
		w * other.w - x * other.x - y * other.y - z * other.z  // w
	);
}

// Are two quaternions equal to each other?
template<typename T>
bool Quaternion<T>::operator==(Quaternion<T> other)
{
	float tolerance = 0.0001f;
	return abs(x - other.x) < tolerance && abs(y - other.y) < tolerance && abs(z - other.z) < tolerance && abs(w - other.w) < tolerance;
}

// Access the x, y, z, w components using [0], [1], [2], [3] respectively.
template<typename T>
T Quaternion<T>::operator[](int index) // Access the x, y, z or w component using [0], [1], [2] or [3] respectively.
{
	if (index == 0) {
		return x;
	}
	if (index == 1) {
		return y;
	}
	if (index == 2) {
		return z;
	}
	if (index == 3) {
		return w;
	}
}