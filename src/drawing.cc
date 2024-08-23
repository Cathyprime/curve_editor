#include "drawing.hh"
#include "consts.hh"
#include "math.hh"
#include <cstddef>
#include <raylib.h>

RaylibDrawing::RaylibDrawing()
{
	BeginDrawing();
	top = Vector2(960, 200);
	bottom_left = Vector2(400, 880);
	bottom_right = Vector2(1520, 880);
}

RaylibDrawing::~RaylibDrawing()
{
	EndDrawing();
}

void RaylibDrawing::operator()()
{
	ClearBackground(WHITE);
	DrawCircleV(top, 10, GRAY);
	DrawCircleV(bottom_left, 10, GRAY);
	DrawCircleV(bottom_right, 10, GRAY);

	DrawLineV(bottom_left, top, GRAY);
	DrawLineV(top, bottom_right, GRAY);

	for (size_t i = 0; i <= 1000; ++i) {
		auto p1 = bezier::lerp(bottom_left, top, i/1000.0f);
		auto p2 = bezier::lerp(top, bottom_right, i/1000.0f);

		auto painter = bezier::lerp(p1, p2, i/1000.0f);

		DrawCircleV(painter, 3, RED);
	}
}
