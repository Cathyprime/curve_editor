#include <raylib.h>
#include "drawing.hh"
#include "consts.hh"
#include "math.hh"

int main()
{
	InitWindow(WIDTH, HEIGHT, "Hello, Raylib!");
	SetTargetFPS(120);

	while (!WindowShouldClose()) {
		RaylibDrawing draw;
		draw();
	}
}
