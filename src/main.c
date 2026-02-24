#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 900
#define HEIGHT 600
#define FPS 60


int main(void)
{
	InitWindow(WIDTH, HEIGHT, "Nucleon simulation");
	SetTargetFPS(FPS);
	
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		EndDrawing();
	}
	
	CloseWindow();

	return 0;
}
