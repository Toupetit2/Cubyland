#include "Vector4.h"

// Returns the length of this vector.
template<typename T>
float Vector4<T>::magnitude() {
	return std::sqrt(x * x + y * y + z * z + w * w);
}

// Returns the squared length of this vector.
template<typename T>
float Vector4<T>::sqrMagnitude() {
	return (x * x + y * y + z * z + w * w);
}

// Returns a normalized vector based on the current vector.
template<typename T>
Vector4<T> Vector4<T>::normalized() {
	return Vector4<T>(x / magnitude(), y / magnitude(), z / magnitude(), w / magnitude());
}

// Makes this vector have a magnitude of 1.
template<typename T>
void Vector4<T>::Normalize()
{
	float mag = magnitude();
	x /= mag;
	y /= mag;
	z /= mag;
	w /= mag;
}

// Returns a normalized vector based on the given vector.
template<typename T>
Vector4<T> Vector4<T>::Normalize(Vector4<T>& value)
{
	float mag = value.magnitude();
	return Vector4<T>(value.x / mag, value.y / mag, value.z / mag, value.w / mag);
}

// Set x, y ,z and w components of an existing Vector4<T>.
template<typename T>
void Vector4<T>::Set(T newX, T newY, T newZ, T newW)
{
	Vector4<T>::x = newX;
	Vector4<T>::y = newY;
	Vector4<T>::z = newZ;
	Vector4<T>::w = newW;
}


// Returns a formatted string for this vector.
template<typename T>
std::string Vector4<T>::ToString(std::string format)
{
	std::stringstream ss;

	if (format[0] == 'G') // General | The more compact of either fixed-point or scientific notation.
	{
		if (format.size() > 1) { // Si precision inscrite en parametre
			if ((log10(x) < -4 || log10(x) > std::stoi(format.substr(1))) || (log10(y) < -4 || log10(y) > std::stoi(format.substr(1))) || (log10(z) < -4 || log10(z) > std::stoi(format.substr(1))) || (log10(w) < -4 || log10(w) > std::stoi(format.substr(1))))
			{
				format = "E" + format.substr(1);
			}
			else {
				format = "F" + format.substr(1);
			}
		}
		else { // If no format parameter
			if ((log10(x) < -4 || log10(x) > 6) || (log10(y) < -4 || log10(y) > 6) || (log10(z) < -4 || log10(z) > 6) || (log10(w) < -4 || log10(w) > 6))
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
		if (format.size() > 1) { ss << "(" << std::scientific << std::setprecision(std::stoi(format.substr(1))) << x << ", " << y << ", " << z << ", " << w << ")";
		}
		else {
			ss << "(" << std::fixed << std::setprecision(6) << x << ", " << y << ", " << z << ", " << w << ")";
		}
	}

	return ss.str();
}

// Dot Product of two vectors.
template<typename T>
float Vector4<T>::Dot(Vector4<T> lhs, Vector4<T> rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

// Linearly interpolates between vectors a and b by t. With T in the range [0, 1]
template<typename T>
Vector4<T> Vector4<T>::Lerp(Vector4<T> a, Vector4<T> b, float t)
{
	t = std::clamp(t, 0.f, 1.f);
	return Vector4<T>(a + ((b - a) * t));
}

// Linearly interpolates between vectors a and b by t.
template<typename T>
Vector4<T> Vector4<T>::LerpUnclamped(Vector4<T> a, Vector4<T> b, float t)
{
	return Vector4<T>(a + ((b - a) * t));
}

// Returns a vector that is made from the largest components of two vectors.
template<typename T>
Vector4<T> Vector4<T>::Max(Vector4<T> lhs, Vector4<T> rhs)
{
	return Vector4<T>(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z), std::max(lhs.w, rhs.w));
}

// Returns a vector that is made from the smallest components of two vectors.
template<typename T>
Vector4<T> Vector4<T>::Min(Vector4<T> lhs, Vector4<T> rhs)
{
	return Vector4<T>(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z), std::min(lhs.w, rhs.w));
}

// Moves a point current towards target.
template<typename T>
Vector4<T> Vector4<T>::MoveTowards(Vector4<T> current, Vector4<T> target, float maxDistanceDelta)
{
	float dx = target.x - current.x;
	float dy = target.y - current.y;
	float dz = target.z - current.z;
	float dw = target.w - current.w;

	float dist = std::sqrt(dx * dx + dy * dy + dz * dz + dw * dw);

	if (dist <= maxDistanceDelta)
	{
		return target;
	}
	return Vector4<T>(current + ((target - current).normalized() * maxDistanceDelta));
}

// Projects a vector onto another vector.
template<typename T>
Vector4<T> Vector4<T>::Project(Vector4<T> vector, Vector4<T> onNormal)
{
	return onNormal * Dot(vector, onNormal) / Dot(onNormal, onNormal);
}

// Returns the distance between a and b.
template<typename T>
float Vector4<T>::Distance(Vector4<T> a, Vector4<T> b)
{
	return (a - b).magnitude();
}

// Adds two vectors.
template<typename T>
Vector4<T> Vector4<T>::operator+(Vector4<T> other)
{
	return Vector4<T>(x + other.x, y + other.y, z + other.z, w + other.w);
}

// Subtracts one vector from another.
template<typename T>
Vector4<T> Vector4<T>::operator-(Vector4<T> other)
{
	return Vector4<T>(x - other.x, y - other.y, z - other.z, w - other.w);
}

// Negates a vector.
template<typename T>
Vector4<T> Vector4<T>::operator-()
{
	return Vector4<T>(-x, -y, -z, -w);
}


// Multiplies a vector by a number.
template<typename T>
Vector4<T> Vector4<T>::operator*(float d) 
{
	return Vector4<T>(x * d, y * d, z * d, w * d);
}

// Multiplies a vector by another vector.
template<typename T>
Vector4<T> Vector4<T>::operator*(Vector4<T> other) 
{
	return Vector4<T>(x * other.x, y * other.y, z * other.z, w * other.w);
}

// Divides a vector by a number.
template<typename T>
Vector4<T> Vector4<T>::operator/(float d) 
{
	return Vector4<T>(x / d, y / d, z / d, w / d);
}

// Divides a vector by another vector.
template<typename T>
Vector4<T> Vector4<T>::operator/(Vector4<T> other) 
{
	return Vector4<T>(x / other.x, y / other.y, z / other.z, w / other.w);
}

// Returns true if two vectors are approximately equal.
template<typename T>
bool Vector4<T>::operator==(Vector4<T> other) 
{
	float tolerance = 0.0001f;
	return abs(x - other.x) < tolerance && abs(y - other.y) < tolerance && abs(z - other.z) < tolerance && abs(w - other.w) < tolerance;
}

// Access the x, y, z, w components using [0], [1], [2], [3] respectively.
template<typename T>
T Vector4<T>::operator[](int index) 
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

