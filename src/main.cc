#include <raylib.h>
#include <string>
#include <vector>

#include "drawing.hh"
#include "math.hh"

#define CIRCLE_RADIUS 10
#define WIDTH 1920
#define HEIGHT 1080

int main()
{
	Vector2 max{.x = 1920, .y = 1080};
	Vector2 min{.x = 0, .y = 0};

	Vector2* dragged_point = nullptr;
	std::vector<Vector2> points;
	Vector2 mouse;

	InitWindow(WIDTH, HEIGHT, "Hello, Raylib!");
	SetTargetFPS(120);

	points.reserve(4);
	points.push_back(Vector2(760, 740));
	points.push_back(Vector2(760, 340));
	points.push_back(Vector2(1160, 340));
	points.push_back(Vector2(1160, 740));

	while (!WindowShouldClose()) {
		mouse = bezier::Clamp(GetMousePosition(), max, min);
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			if (dragged_point == nullptr) {
				for (auto& point : points) {
					if (CheckCollisionPointCircle(mouse, point, CIRCLE_RADIUS + 10)) {
						dragged_point = &point;
						break;
					}
				}
			} else {
				*dragged_point = mouse;
			}
		} else {
			dragged_point = nullptr;
		}
		{
			RaylibDrawing draw;
			ClearBackground(BLACK);

			for (const auto& point : points) {
				bool hover = CheckCollisionPointCircle(mouse, point, 10);
				DrawCircleV(point, 10, hover ? YELLOW : RED);
			}

			DrawText(std::to_string(mouse.x).c_str(), 0, 0, 50, WHITE);
			DrawText(std::to_string(mouse.y).c_str(), 0, 50, 50, WHITE);

			for (size_t i = 0; i <= 1000; ++i)
				DrawCircleV(bezier::Laura_Cubic_Polynomial(points[0], points[1], points[2], points[3], i / 1000.f), 3,
							YELLOW);

			DrawLineV(points[0], points[1], YELLOW);
			DrawLineV(points[2], points[3], YELLOW);
		}
	}
}
