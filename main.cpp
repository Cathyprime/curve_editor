#include <raylib.h>

const int WIDTH = 1920;
const int HEIGHT = 1080;
const char* TEXT = "Hello from raylib";

int main()
{
	InitWindow(WIDTH, HEIGHT, "Hello, Raylib!");
	SetTargetFPS(120);
	int text_width = MeasureText(TEXT, 200);

	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(WHITE);
		DrawText(TEXT, WIDTH / 2 - text_width / 2, HEIGHT / 2 - 100, 200, RED);
		DrawFPS(0, 0);
		EndDrawing();
	}
}
