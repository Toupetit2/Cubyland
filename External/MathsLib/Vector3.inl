// Vector3(0, -1, 0)
template<typename T>
const Vector3<T> Vector3<T>::down = Vector3<T>(0.f, -1.f, 0.f);
// Vector3(0, 1, 0)
template<typename T>
const Vector3<T> Vector3<T>::up = Vector3<T>(0.f, 1.f, 0.f);
// Vector3(-1, 0, 0)
template<typename T>
const Vector3<T> Vector3<T>::left = Vector3<T>(-1.f, 0.f, 0.f);
// Vector3(1, 0, 0)
template<typename T>
const Vector3<T> Vector3<T>::right = Vector3<T>(1.f, 0.f, 0.f);
// Vector3(0, 0, -1)
template<typename T>
const Vector3<T> Vector3<T>::back = Vector3<T>(0.f, 0.f, -1.f);
// Vector3(0, 0, 1)
template<typename T>
const Vector3<T> Vector3<T>::forward = Vector3<T>(0.f, 0.f, 1.f);
// Vector3(1, 1, 1)
template<typename T>
const Vector3<T> Vector3<T>::one = Vector3<T>(1.f, 1.f, 1.f);
// Vector3(0, 0, 0)
template<typename T>
const Vector3<T> Vector3<T>::zero = Vector3<T>(0.f, 0.f, 0.f);
// Vector3(-inf, -inf, -inf)
template<typename T>
const Vector3<T> Vector3<T>::negativeInfinity = Vector3<T>(-std::numeric_limits<T>::infinity, -std::numeric_limits<T>::infinity, std::numeric_limits<T>::infinity);
// Vector3(inf, inf, inf)
template<typename T>
const Vector3<T> Vector3<T>::positiveInfinity = Vector3<T>(std::numeric_limits<T>::infinity, std::numeric_limits<T>::infinity, std::numeric_limits<T>::infinity);

// Returns the length of this vector.
template<typename T>
float Vector3<T>::magnitude() { 
	return std::sqrt(x * x + y * y + z * z);
}

// Returns the squared length of this vector.
template<typename T>
float Vector3<T>::sqrMagnitude() {
	return (x * x + y * y + z * z);
}

// Returns a normalized vector based on the current vector.
template<typename T>
Vector3<T> Vector3<T>::normalized() { 
	return Vector3<T>(x / magnitude(), y / magnitude(), z / magnitude());
}

// Makes this vector have a magnitude of 1.
template<typename T>
void Vector3<T>::Normalize() 
{
	float mag = magnitude();
	x /= mag;
	y /= mag;
	z /= mag;
}

// Returns a normalized vector based on the given vector.
template<typename T>
Vector3<T> Vector3<T>::Normalize(Vector3<T>& value)
{
	float mag = value.magnitude();
	return Vector3<T>(value.x / mag, value.y / mag, value.z / mag);
}

// Set x, y and z components of an existing Vector3<T>.
template<typename T>
void Vector3<T>::Set(T newX, T newY, T newZ) 
{
	Vector3<T>::x = newX;
	Vector3<T>::y = newY;
	Vector3<T>::z = newZ;
}

// Returns a formatted string for this vector.
template<typename T>
std::string Vector3<T>::ToString(std::string format) 
{
	std::stringstream ss;

	if (format[0] == 'G') // General | The more compact of either fixed-point or scientific notation.
	{
		if (format.size() > 1) { // Si precision inscrite en parametre
			if ((log10(x) < -4 || log10(x) > std::stoi(format.substr(1))) || (log10(y) < -4 || log10(y) > std::stoi(format.substr(1)) || (log10(z) < -4 || log10(z) > std::stoi(format.substr(1)))))
			{
				format = "E" + format.substr(1);
			}
			else {
				format = "F" + format.substr(1);
			}
		}
		else { // If no format parameter
			if ((log10(x) < -4 || log10(x) > 6 || (log10(y) < -4 || log10(y) > 6) || (log10(z) < -4 || log10(z) > 6)))
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
			ss << "(" << std::fixed << std::setprecision(std::stoi(format.substr(1))) << x << ", " << y << ", " << z << ")";
		}
		else {
			ss << "(" << std::fixed << std::setprecision(2) << x << ", " << y << ", " << z << ")";
		}
	}

	else if (format[0] == 'E') // Exponential (scientific) | Exponential notation.
	{
		if (format.size() > 1) {
			ss << "(" << std::scientific << std::setprecision(std::stoi(format.substr(1))) << x << ", " << y << ", " << z << ")";
		}
		else {
			ss << "(" << std::fixed << std::setprecision(6) << x << ", " << y << ", " << z << ")";
		}
	}

	return ss.str();
}

// Gets the unsigned angle in degrees between from and to.
template<typename T>
float Vector3<T>::Angle(Vector3<T> from, Vector3<T> to) 
{
	float ax = from.x;
	float bx = to.x;
	float ay = from.y;
	float by = to.y;
	float az = from.z;
	float bz = to.z;

	float dot = ax * bx + ay * by + az * bz;

	float cosTheta = dot / (std::sqrt(ax * ax + ay * ay + az * bz) * std::sqrt(bx * bx + by * by + az * bz));
	if (cosTheta > 1) cosTheta = 1;
	if (cosTheta < -1) cosTheta = -1;

	return  acos(cosTheta) * 180 / M_PI;
}

// Returns the distance between a and b.
template<typename T>
float Vector3<T>::Distance(Vector3<T> a, Vector3<T> b)
{
	float dx = b.x - a.x;
	float dy = b.y - a.y;
	float dz = b.z - a.z;
	return std::sqrt(dx * dx + dy * dy + dz * dz);
}

// Returns a copy of vector with its magnitude clamped to maxLength.
template<typename T>
Vector3<T> Vector3<T>::ClampMagnitude(Vector3<T> vector, float maxLength) 
{
	float mag = vector.magnitude();
	if (mag > maxLength && mag > 0) {
		return vector.normalized() * maxLength;
	}
	return vector;
}

// 	Calculates the cross product of two three-dimensional vectors.
template<typename T>
Vector3<T> Vector3<T>::Cross(Vector3 lhs, Vector3 rhs)
{
	return Vector3<T>(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
}

// Dot Product of two vectors.
template<typename T>
float Vector3<T>::Dot(Vector3<T> lhs, Vector3<T> rhs) 
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

// Linearly interpolates between vectors a and b by t. With T in the range [0, 1]
template<typename T>
Vector3<T> Vector3<T>::Lerp(Vector3<T> a, Vector3<T> b, float t)
{
	t = std::clamp(t, 0.f, 1.f);
	return Vector3<T>(a + ((b - a) * t));
}

// Linearly interpolates between vectors a and b by t.
template<typename T>
Vector3<T> Vector3<T>::LerpUnclamped(Vector3<T> a, Vector3<T> b, float t)
{
	return Vector3<T>(a + ((b - a) * t));
}

// Returns a vector that is made from the largest components of two vectors.
template<typename T>
Vector3<T> Vector3<T>::Max(Vector3<T> lhs, Vector3<T> rhs) 
{
	return Vector3<T>(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z));
}

// Returns a vector that is made from the smallest components of two vectors.
template<typename T>
Vector3<T> Vector3<T>::Min(Vector3<T> lhs, Vector3<T> rhs) 
{
	return Vector3<T>(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z));
}

// Moves a point current towards target.
template<typename T>
Vector3<T> Vector3<T>::MoveTowards(Vector3<T> current, Vector3<T> target, float maxDistanceDelta) 
{
	float dx = target.x - current.x;
	float dy = target.y - current.y;
	float dz = target.z - current.z;
	float dist = std::sqrt(dx * dx + dy * dy + dz *dz);

	if (dist <= maxDistanceDelta)
	{
		return target;
	}
	return Vector3<T>(current + ((target - current).normalized() * maxDistanceDelta));
}

// Makes vectors normalized and orthogonal to each other.
template<typename T>
void Vector3<T>::OrthoNormalize(Vector3& normal, Vector3& tangent)
{
	normal.Normalize();
	tangent = tangent - normal * Vector3::Dot(normal, tangent);
	tangent.Normalize();
}

// Makes vectors normalized and orthogonal to each other.
template<typename T>
void Vector3<T>::OrthoNormalize(Vector3& normal, Vector3& tangent, Vector3& binormal)
{
	normal.Normalize();
	tangent = tangent - normal * Vector3::Dot(normal, tangent);
	tangent.Normalize();
	binormal = binormal - normal * Vector3::Dot(normal, binormal) - tangent * Vector3::Dot(tangent, binormal);
	binormal.Normalize();
}

// Projects a vector onto another vector.
template<typename T>
Vector3<T> Vector3<T>::Project(Vector3 vector, Vector3 onNormal) 
{
	return onNormal * Dot(vector, onNormal) / Dot(onNormal, onNormal);
}

// Projects a vector onto a plane.
template<typename T>
Vector3<T> Vector3<T>::ProjectOnPlane(Vector3 vector, Vector3 planeNormal)
{
	return vector - (planeNormal * Dot(vector, planeNormal) / Dot(planeNormal, planeNormal));
}

// Rotates a vector current towards target.
template<typename T>
Vector3<T> Vector3<T>::RotateTowards(Vector3 current, Vector3 target, float maxRadiansDelta, float maxMagnitudeDelta)
{
	Vector3 cDir = current.normalized();
	Vector3 tDir = target.normalized();

	float angle = std::acos(Dot(cDir, tDir));

	if (angle <= maxRadiansDelta) {
		return target;
	}

	Vector3 rotatedDir = SlerpUnclamped(current, target, std::min(1.f, maxRadiansDelta / angle));

	float cMag = current.magnitude();
	float tMag = target.magnitude();

	float newMag = cMag + std::clamp(tMag - cMag, -maxMagnitudeDelta, maxMagnitudeDelta);

	return rotatedDir * newMag;
}

// Spherically interpolates between two three-dimensional vectors.
template<typename T>
Vector3<T> Vector3<T>::Slerp(Vector3 a, Vector3 b, float t)
{
	return SlerpUnclamped(a, b, std::clamp(t, 0.f, 1.f));
}

// Spherically interpolates between two vectors.
template<typename T>
Vector3<T> Vector3<T>::SlerpUnclamped(Vector3 a, Vector3 b, float t)
{
	float theta = std::acos(Dot(a.normalized(), b.normalized()));

	return a.normalized() * std::sin((1 - t) * theta) / sin(theta) + b.normalized() * sin(t * theta) / sin(theta);
}

// Reflects a vector off the plane defined by a normal.
template<typename T>
Vector3<T> Vector3<T>::Reflect(Vector3<T> inDirection, Vector3<T> inNormal)
{
	return inDirection - inNormal * 2 * Dot(inDirection, inNormal);
}

// Multiplies two vectors component-wise.
template<typename T>
Vector3<T> Vector3<T>::Scale(Vector3<T> a, Vector3<T> b) 
{
	return a * b;
}

// Multiplies every component of this vector by the same component of scale.
template<typename T>
void Vector3<T>::Scale(Vector3<T> scale)
{
	x *= scale.x;
	y *= scale.y;
	z *= scale.z;
}

// Gets the signed angle in degrees between from and to.
template<typename T>
float Vector3<T>::SignedAngle(Vector3<T> from, Vector3<T> to, Vector3<T> axis) 
{
	return atan2(from.x * to.y - from.y * to.x, from.x * to.x + from.y * to.y) * 180 / M_PI;
}

// Adds two vectors.
template<typename T>
Vector3<T> Vector3<T>::operator+(Vector3<T> other) 
{
	return Vector3<T>(x + other.x, y + other.y, z + other.z);
}

// Subtracts one vector from another.
template<typename T>
Vector3<T> Vector3<T>::operator-(Vector3<T> other) 
{
	return Vector3<T>(x - other.x, y - other.y, z - other.z);
}

// Negates a vector.
template<typename T>
Vector3<T> Vector3<T>::operator-() 
{
	return Vector3<T>(-x, -y, -z);
}

// Multiplies a vector by a number.
template<typename T>
Vector3<T> Vector3<T>::operator*(float d) 
{
	return Vector3<T>(x * d, y * d, z * d);
}

// Multiplies a vector by another vector.
template<typename T>
Vector3<T> Vector3<T>::operator*(Vector3<T> other) 
{
	return Vector3<T>(x * other.x, y * other.y, z * other.z);
}

// Divides a vector by a number.
template<typename T>
Vector3<T> Vector3<T>::operator/(float d) 
{
	return Vector3<T>(x / d, y / d, z / d);
}

// Divides a vector by another vector.
template<typename T>
Vector3<T> Vector3<T>::operator/(Vector3<T> other) 
{
	return Vector3<T>(x / other.x, y / other.y, z / other.z);
}

// Returns true if two vectors are approximately equal.
template<typename T>
bool Vector3<T>::operator==(Vector3<T> other) 
{
	float tolerance = 0.0001f;
	return abs(x - other.x) < tolerance && abs(y - other.y) < tolerance && abs(z - other.z) < tolerance;
}

// Access the x, y or z component using [0], [1] or [2] respectively.
template<typename T>
T Vector3<T>::operator[](int index) 
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
}