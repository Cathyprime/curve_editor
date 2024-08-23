#include <raylib.h>

Vector2 operator*(const Vector2& vec, float scalar);
Vector2 operator*(float scalar, const Vector2& vec);
Vector2 operator+(const Vector2& lhs, const Vector2& rhs);

namespace bezier {
Vector2 lerp(Vector2 P0, Vector2 P1, float t);
}
