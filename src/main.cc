#include <raylib.h>
#include "drawing.hh"
#include "consts.hh"

int main()
{
	InitWindow(WIDTH, HEIGHT, "Hello, Raylib!");
	SetTargetFPS(120);

	while (!WindowShouldClose()) {
		RaylibDrawing drawing;
		drawing();
	}
}
