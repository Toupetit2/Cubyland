// Vector2(0, -1)
template<typename T>
const Vector2<T> Vector2<T>::down = Vector2<T>(0.f, -1.f);
// Vector2(0, 1)
template<typename T>
const Vector2<T> Vector2<T>::up = Vector2<T>(0.f, 1.f);
// Vector2(-1, 0)
template<typename T>
const Vector2<T> Vector2<T>::left = Vector2<T>(-1.f, 0.f);
// Vector2(1, 0)
template<typename T>
const Vector2<T> Vector2<T>::right = Vector2<T>(1.f, 0.f);
// Vector2(1, 1)
template<typename T>
const Vector2<T> Vector2<T>::one = Vector2<T>(1.f, 1.f);
// Vector2(0, 0)
template<typename T>
const Vector2<T> Vector2<T>::zero = Vector2<T>(0.f, 0.f);
// Vector2(-inf, -inf)
template<typename T>
const Vector2<T> Vector2<T>::negativeInfinity = Vector2<T>(-std::numeric_limits<T>::infinity, -std::numeric_limits<T>::infinity);
// Vector2(inf, inf)
template<typename T>
const Vector2<T> Vector2<T>::positiveInfinity = Vector2<T>(std::numeric_limits<T>::infinity, std::numeric_limits<T>::infinity);

// Returns the length of this vector.
template<typename T>
float Vector2<T>::magnitude() { 
	return std::sqrt(x * x + y * y);
}

// Returns the squared length of this vector.
template<typename T>
float Vector2<T>::sqrMagnitude() { 
	return (x * x + y * y);
}

// Returns a normalized vector based on the current vector.
template<typename T>
Vector2<T> Vector2<T>::normalized() {
	return Vector2<T>(x / magnitude(), y / magnitude());
}

// Makes this vector have a magnitude of 1.
template<typename T>
void Vector2<T>::Normalize() 
{
	float mag = magnitude();
	x /= mag;
	y /= mag;
}

// Set x and y components of an existing Vector2<T>.
template<typename T>
void Vector2<T>::Set(T newX, T newY) 
{
	Vector2<T>::x = newX;
	Vector2<T>::y = newY;
}

// Returns a formatted string for this vector.
template<typename T>
std::string Vector2<T>::ToString(std::string format) 
{
	std::stringstream ss;

	if (format[0] == 'G') // General | The more compact of either fixed-point or scientific notation.
	{
		if (format.size() > 1) { // Si precision inscrite en parametre
			if ((log10(x) < -4 || log10(x) > std::stoi(format.substr(1))) || (log10(y) < -4 || log10(y) > std::stoi(format.substr(1))))
			{
				format = "E" + format.substr(1);
			}
			else {
				format = "F" + format.substr(1);
			}
		}
		else { // Si pas de precision
			if ((log10(x) < -4 || log10(x) > 6 || (log10(y) < -4 || log10(y) > 6)))
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
			ss << "(" << std::fixed << std::setprecision(std::stoi(format.substr(1))) << x << ", " << y << ")";
		}
		else {
			ss << "(" << std::fixed << std::setprecision(2) << x << ", " << y << ")";
		}
	}

	else if (format[0] == 'E') // Exponential (scientific) | Exponential notation.
	{
		if (format.size() > 1) {
			ss << "(" << std::scientific << std::setprecision(std::stoi(format.substr(1))) << x << ", " << y << ")";
		}
		else {
			ss << "(" << std::fixed << std::setprecision(6) << x << ", " << y << ")";
		}
	}

	return ss.str();
}

// Gets the unsigned angle in degrees between from and to.
template<typename T>
float Vector2<T>::Angle(Vector2<T> from, Vector2<T> to) 
{
	float ax = from.x;
	float bx = to.x;
	float ay = from.y;
	float by = to.y;

	float dot = ax * bx + ay * by;

	float cosTheta = dot / (std::sqrt(ax * ax + ay * ay) * std::sqrt(bx * bx + by * by));
	if (cosTheta > 1) cosTheta = 1;
	if (cosTheta < -1) cosTheta = -1;
	
	return  acos(cosTheta) * 180 / M_PI;
}

// Returns the distance between a and b.
template<typename T>
float Vector2<T>::Distance(Vector2<T> a, Vector2<T> b) 
{
	float dx = b.x - a.x;
	float dy = b.y - a.y;
	return std::sqrt(dx * dx + dy * dy);
}

// Returns a copy of vector with its magnitude clamped to maxLength.
template<typename T>
Vector2<T> Vector2<T>::ClampMagnitude(Vector2<T> vector, float maxLength) 
{
	float mag = vector.magnitude();
	if (mag > maxLength && mag > 0) {
		return vector.normalized() * maxLength;
	}
	return vector;
}

// Dot Product of two vectors.
template<typename T>
float Vector2<T>::Dot(Vector2<T> lhs, Vector2<T> rhs) 
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

// Linearly interpolates between vectors a and b by t. With T in the range [0, 1]
template<typename T>
Vector2<T> Vector2<T>::Lerp(Vector2<T> a, Vector2<T> b, float t) 
{
	t = std::clamp(t, 0.f, 1.f);
	return Vector2<T>(a + ((b - a) * t));
}

// Linearly interpolates between vectors a and b by t.
template<typename T>
Vector2<T> Vector2<T>::LerpUnclamped(Vector2<T> a, Vector2<T> b, float t) 
{
	return Vector2<T>(a + ((b - a) * t));
}

// Returns a vector that is made from the largest components of two vectors.
template<typename T>
Vector2<T> Vector2<T>::Max(Vector2<T> lhs, Vector2<T> rhs) 
{
	return Vector2<T>(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y));
}

// Returns a vector that is made from the smallest components of two vectors.
template<typename T>
Vector2<T> Vector2<T>::Min(Vector2<T> lhs, Vector2<T> rhs) 
{
	return Vector2<T>(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y));
}

// Moves a point current towards target.
template<typename T>
Vector2<T> Vector2<T>::MoveTowards(Vector2<T> current, Vector2<T> target, float maxDistanceDelta) 
{
	float dx = target.x - current.x;
	float dy = target.y - current.y;
	float dist = std::sqrt(dx * dx + dy * dy);

	if (dist <= maxDistanceDelta)
	{
		return target;
	}
	return Vector2<T>(current + ((target - current).normalized() * maxDistanceDelta));
}

// Returns the 2D vector perpendicular to this 2D vector.
template<typename T>
Vector2<T> Vector2<T>::Perpendicular(Vector2<T> inDirection) 
{
	return Vector2<T>(-inDirection.y, inDirection.x);
}

// Reflects a vector off the surface defined by a normal.
template<typename T>
Vector2<T> Vector2<T>::Reflect(Vector2<T> inDirection, Vector2<T> inNormal) 
{
	return inDirection - inNormal * 2 * Dot(inDirection, inNormal);
}

// Multiplies two vectors component-wise.
template<typename T>
Vector2<T> Vector2<T>::Scale(Vector2<T> a, Vector2<T> b) 
{
	return a * b;
}

// Multiplies two vectors component-wise.
template<typename T>
void Vector2<T>::Scale(Vector2<T> scale)
{
	x *= scale.x;
	y *= scale.y;
}

// Gets the signed angle in degrees between from and to.
template<typename T>
float Vector2<T>::SignedAngle(Vector2<T> from, Vector2<T> to) 
{
	return atan2(from.x * to.y - from.y * to.x, from.x * to.x + from.y * to.y) * 180 / M_PI;
}

// Adds two vectors.
template<typename T>
Vector2<T> Vector2<T>::operator+(Vector2<T> other) 
{
	return Vector2<T>(x + other.x, y + other.y);
}

// Subtracts one vector from another.
template<typename T>
Vector2<T> Vector2<T>::operator-(Vector2<T> other) 
{
	return Vector2<T>(x - other.x, y - other.y);
}

// Negates a vector.
template<typename T>
Vector2<T> Vector2<T>::operator-() 
{
	return Vector2<T>(-x, -y);
}

// Multiplies a vector by a number.
template<typename T>
Vector2<T> Vector2<T>::operator*(float d)
{
	return Vector2<T>(x * d, y * d);
}

// Multiplies a vector by another vector.
template<typename T>
Vector2<T> Vector2<T>::operator*(Vector2<T> other) 
{
	return Vector2<T>(x * other.x, y * other.y);
}

// Divides a vector by a number.
template<typename T>
Vector2<T> Vector2<T>::operator/(float d)
{
	return Vector2<T>(x / d, y / d);
}

// Divides a vector by another vector.
template<typename T>
Vector2<T> Vector2<T>::operator/(Vector2<T> other) 
{
	return Vector2<T>(x / other.x, y / other.y);
}

// Returns true if two vectors are approximately equal.
template<typename T>
bool Vector2<T>::operator==(Vector2<T> other) 
{
	float tolerance = 0.0001f;
	return abs(x - other.x) < tolerance && abs(y - other.y) < tolerance;
}

// Access the x or y component using [0] or [1] respectively.
template<typename T>
T Vector2<T>::operator[](int index) 
{
	if (index == 0) {
		return x;
	}
	if (index == 1) {
		return y;
	}
}