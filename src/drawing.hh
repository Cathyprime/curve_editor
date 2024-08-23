#include <raylib.h>
#include <vector>

class RaylibDrawing
{
  private:
	static std::vector<Vector2> points;
	static Vector2* dragged_point;
	static Vector2 mouse;
	static bool initialized;

  public:
	RaylibDrawing();
	~RaylibDrawing();

	void operator()();
	void draw();
	void update();
};
