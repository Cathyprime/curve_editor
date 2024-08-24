#include "math.hh"
#include <algorithm>
#include <raylib.h>

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

Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
{
	return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
}

Vector2 operator-(const Vector2& vec)
{
	return Vector2(-vec.x, -vec.y);
}

namespace bezier {
Vector2 lerp(Vector2 P0, Vector2 P1, float t)
{
	return ((1 - t) * P0) + (t * P1);
}

Vector2 Laura_Cubic_Polynomial(Vector2 P0, Vector2 P1, Vector2 P2, Vector2 P3, float t)
{
	auto t_3 = t * t * t;
	auto t_2 = t * t;

	return t_3 * (-P0 + 3 * P1 - 3 * P2 + P3)
		 + t_2 * (3 * P0 - 6 * P1 + 3 * P2)
		 + t * (-3 * P0 + 3 * P1)
		 + P0;
}

Vector2 Clamp(Vector2 vec, Vector2 max, Vector2 min)
{
	return Vector2{
		.x = std::min(std::max(vec.x, min.x), max.x),
		.y = std::min(std::max(vec.y, min.y), max.y),
	};
}

} // namespace bezier
