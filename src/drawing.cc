#include "drawing.hh"
#include "consts.hh"

void RaylibDrawing::operator()()
{
	ClearBackground(WHITE);
	DrawText(TEXT, WIDTH / 2 - MeasureText(TEXT, 200) / 2, HEIGHT / 2 - 100, 200, RED);
	DrawFPS(0, 0);
}
