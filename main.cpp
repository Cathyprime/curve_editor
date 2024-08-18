#include "width.hh"
#include "height.hh"
#include "text.hh"
#include "title.hh"
#include <raylib.h>

const int WIDTH = width();
const int HEIGHT = height();

int main()
{
	InitWindow(WIDTH, HEIGHT, title());
	SetTargetFPS(120);
	int text_width = MeasureText(text(), 200);

	while(!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(WHITE);

		DrawText(text(), WIDTH/2 - text_width/2, HEIGHT/2 - 100, 200, RED);
		DrawFPS(0, 0);
		EndDrawing();
	}
}
