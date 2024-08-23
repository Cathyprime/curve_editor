#include "drawing.hh"
#include "consts.hh"
#include "math.hh"
#include <raylib.h>
#include <string>

Vector2* RaylibDrawing::dragged_point = nullptr;
std::vector<Vector2> RaylibDrawing::points;
Vector2 RaylibDrawing::mouse;
bool RaylibDrawing::initialized = false;

#define CIRCLE_RADIUS 10

RaylibDrawing::RaylibDrawing()
{
	BeginDrawing();
	if (!initialized) {
		points.reserve(4);
		points.push_back(Vector2(760, 740));
		points.push_back(Vector2(760, 340));
		points.push_back(Vector2(1160, 340));
		points.push_back(Vector2(1160, 740));
		initialized = true;
	}
}

RaylibDrawing::~RaylibDrawing()
{
	EndDrawing();
}

void RaylibDrawing::operator()()
{
	update();
	draw();
}

void RaylibDrawing::update()
{
}

void RaylibDrawing::draw()
{
	ClearBackground(BLACK);

	for (const auto& point : points) {
		bool hover = CheckCollisionPointCircle(mouse, point, 10);
		DrawCircleV(point, 10, hover ? YELLOW : RED);
	}

	DrawText(std::to_string(mouse.x).c_str(), 0, 0, 50, WHITE);
	DrawText(std::to_string(mouse.y).c_str(), 0, 50, 50, WHITE);

	for (size_t i = 0; i <= 1000; ++i)
		DrawCircleV(bezier::Laura_Cubic_Polynomial(points[0], points[1], points[2], points[3], i / 1000.f), 3, YELLOW);

	DrawLineV(points[0], points[1], YELLOW);
	DrawLineV(points[2], points[3], YELLOW);
}
