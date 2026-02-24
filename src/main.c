#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


// --- CONSTANTS ------------>

#define WIDTH 900
#define HEIGHT 600
#define FPS 60
#define RADIUS 5
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

Vec2 Subtract(Vec2, Vec2);
float Dot(Vec2, Vec2);

void InitNucleons(void);
void ZeroOutForces(void);
void ComputeForces(void);
void UpdatePositions(float);
void DrawNucleons(void);


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
			ZeroOutForces();
			ComputeForces();
			UpdatePositions(GetFrameTime());
			DrawNucleons();
		EndDrawing();
	}
	
	CloseWindow();

	return 0;
}


// --- IMPLEMENTATIONS ------------>

Vec2 Subtract(Vec2 a, Vec2 b)
{
	Vec2 res;
	res.x = a.x - b.x;
	res.y = a.y - b.y;
	return res;
}

float Dot(Vec2 a, Vec2 b)
{
    return a.x * b.x + a.y * b.y;
}

void InitNucleons(void)
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

void ZeroOutForces(void)
{
	for (int i = 0; i < NUM_NUCLEONS; i++) nucleons[i].force = (Vec2){0, 0};
}

void ComputeForces(void)
{
	Nucleon *self, *other;

	for (int i = 0; i < NUM_NUCLEONS; i++)
	{
		self = &nucleons[i];

		for (int j = i+1; j < NUM_NUCLEONS; j++)
		{
			other = &nucleons[j];

			Vec2  delta            = Subtract(other->position, self->position);
			float distance_squared = Dot(delta, delta);
			if (distance_squared < 1.0f) distance_squared = 1.0f;
			float inv_distance = 1.0f / sqrtf(distance_squared);
			Vec2 normal = { delta.x * inv_distance, delta.y * inv_distance };

			float coulomb = self->charge * other->charge / distance_squared;

			self->force.x += coulomb * normal.x;
			self->force.y += coulomb * normal.y;

			other->force.x -= coulomb * normal.x;
			other->force.y -= coulomb * normal.y;
		}
	}
}

void UpdatePositions(float dt)
{
	for (int i = 0; i < NUM_NUCLEONS; i++)
	{
		nucleons[i].velocity.x += nucleons[i].force.x * dt;
		nucleons[i].velocity.y += nucleons[i].force.y * dt;

		nucleons[i].position.x += nucleons[i].velocity.x * dt;
		nucleons[i].position.y += nucleons[i].velocity.y * dt;
	}
}

void DrawNucleons(void)
{
	Color color;
	for (int i = 0; i < NUM_NUCLEONS; i++)
	{
		color = nucleons[i].charge == CHARGE_POSITIVE ? BLUE : RED;
		DrawCircle(nucleons[i].position.x, nucleons[i].position.y, nucleons[i].radius, color);
	}
}