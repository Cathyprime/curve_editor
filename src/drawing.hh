#include <raylib.h>

class RaylibDrawing
{
  private:
	Vector2 top;
	Vector2 bottom_left;
	Vector2 bottom_right;

  public:
	RaylibDrawing();
	~RaylibDrawing();

	void operator()();
};
