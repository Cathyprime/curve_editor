#include "math.hh"

Vector2 operator*(const Vector2& vec, float scalar)
{
	return Vector2(vec.x * scalar, vec.y * scalar);
}

Vector2 operator*(float scalar, const Vector2& vec)
{
	return vec * scalar;
}

Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
{
	return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
}

namespace bezier {
Vector2 lerp(Vector2 P0, Vector2 P1, float t)
{
	return (P0 * (1 - t)) + (P1 * t);
}
} // namespace bezier
