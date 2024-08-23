#include <raylib.h>

class RaylibDrawing
{

  public:
	RaylibDrawing()
	{
		BeginDrawing();
	}
	~RaylibDrawing()
	{
		EndDrawing();
	}

	void operator()();
};
