#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


// --- CONSTANTS ------------>

#define WIDTH 900
#define HEIGHT 600
#define FPS 60
#define RADIUS 10
#define NUM_NUCLEONS 30
#define SPEED 20


// --- STRUCTURES ------------>

typedef struct s_vec2
{
	float x, y;
} Vec2;

typedef enum e_charge {
    CHARGE_NEGATIVE = -1,
    CHARGE_POSITIVE = 1
} Charge;

typedef struct s_nucleon
{
	Vec2 position, velocity, force;
	float radius;
	Charge charge;
} Nucleon;


// --- PROTOTYPES ------------>

void InitNucleons();


// --- ENTRY POINT ------------>

Nucleon nucleons[NUM_NUCLEONS];

int main(void)
{
	InitWindow(WIDTH, HEIGHT, "Nucleon simulation");
	SetTargetFPS(FPS);

	InitNucleons();
	
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

void InitNucleons()
{
	for (int i = 0; i < NUM_NUCLEONS; i++)
	{
		nucleons[i].radius = RADIUS;
		nucleons[i].position.x = GetRandomValue(RADIUS, WIDTH-RADIUS);
		nucleons[i].position.y = GetRandomValue(RADIUS, HEIGHT-RADIUS);
		nucleons[i].velocity.x = GetRandomValue(-SPEED, SPEED);
		nucleons[i].velocity.y = GetRandomValue(-SPEED, SPEED);
		nucleons[i].force.x = 0;
		nucleons[i].force.y = 0;
		nucleons[i].charge = CHARGE_POSITIVE;
	}
}