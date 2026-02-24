#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


// --- CONSTANTS ------------>
#define WIDTH 900
#define HEIGHT 600
#define FPS 60


// --- STRUCTURES ------------>
typedef struct s_pair
{
	float x, y;
} Pair;

typedef enum e_charge {
    CHARGE_NEGATIVE = -1,
    CHARGE_POSITIVE = 1
} Charge;

typedef struct s_nucleon
{
	Pair position, velocity, force;
	float radius;
	Charge charge;
} Nucleon;


// --- PROTOTYPES ------------>

// --- ENTRY POINT ------------>
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


// --- IMPLEMENTATIONS ------------>