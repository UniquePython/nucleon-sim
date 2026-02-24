#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


// --- CONSTANTS ------------>

#define WIDTH 1500
#define HEIGHT 1000
#define FPS 60
#define RADIUS 5
#define NUM_NUCLEONS 500
#define DISTANCE_SCALE 1e-15
#define TIME_SCALE 2e-22


// --- STRUCTURES ------------>

typedef struct s_vec2
{
	double x, y;
} Vec2;

typedef enum e_charge {
    CHARGE_NEGATIVE = -1,
	CHARGE_NEUTRAL = 0,
    CHARGE_POSITIVE = 1
} Charge;

typedef struct s_nucleon
{
	Vec2 position, velocity, force;
	double radius;
	Charge charge;
	double mass;
} Nucleon;


// --- PROTOTYPES ------------>

Vec2 Subtract(Vec2, Vec2);
double Dot(Vec2, Vec2);

double ChargeToCoulombs(Charge);

void InitNucleons(void);
void ZeroOutForces(void);
void ComputeForces(void);
void UpdatePositions(double);
void CollideWithWalls(void);
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
			CollideWithWalls();
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

double Dot(Vec2 a, Vec2 b)
{
    return a.x * b.x + a.y * b.y;
}

double ChargeToCoulombs(Charge c)
{
    switch(c)
    {
        case CHARGE_NEGATIVE: return -1.602e-19;
        case CHARGE_NEUTRAL:  return 0.0;
        case CHARGE_POSITIVE: return 1.602e-19;
    }
}

void InitNucleons(void)
{
	for (int i = 0; i < NUM_NUCLEONS; i++)
	{
		nucleons[i].radius = RADIUS;
		nucleons[i].position.x = GetRandomValue(RADIUS, WIDTH-RADIUS);
		nucleons[i].position.y = GetRandomValue(RADIUS, HEIGHT-RADIUS);
		nucleons[i].velocity = (Vec2){0, 0};
		nucleons[i].force = (Vec2){0, 0};

		int rand = GetRandomValue(-1,1);
		switch (rand)
		{
			case -1:
				nucleons[i].charge = CHARGE_NEGATIVE;
				break;
			
			case 0:
				nucleons[i].charge = CHARGE_NEUTRAL;
				break;
			
			case 1:
				nucleons[i].charge = CHARGE_POSITIVE;
				break;
		}

		switch (nucleons[i].charge)
		{
			case CHARGE_NEGATIVE:
				nucleons[i].mass = 9.109e-31;
				break;
			
			case CHARGE_NEUTRAL:
				nucleons[i].mass = 1.675e-27;
				break;
			
			case CHARGE_POSITIVE:
				nucleons[i].mass = 1.673e-27;
				break;
		}
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

			Vec2 delta_pixels = Subtract(self->position, other->position);
			Vec2 delta_meters = { delta_pixels.x * DISTANCE_SCALE, delta_pixels.y * DISTANCE_SCALE };
			double distance_squared = Dot(delta_meters, delta_meters);
			if (distance_squared < 1e-30) distance_squared = 1e-30;
			double inv_distance = 1.0 / sqrt(distance_squared);
			Vec2 normal = { delta_meters.x * inv_distance, delta_meters.y * inv_distance };

			double k_e = 8.988e9;
			double coulomb = k_e * ChargeToCoulombs(self->charge) * ChargeToCoulombs(other->charge) / distance_squared;

			self->force.x += coulomb * normal.x;
			self->force.y += coulomb * normal.y;

			other->force.x -= coulomb * normal.x;
			other->force.y -= coulomb * normal.y;
		}
	}
}

void UpdatePositions(double dt)
{
	for (int i = 0; i < NUM_NUCLEONS; i++)
	{
		nucleons[i].velocity.x += (nucleons[i].force.x / nucleons[i].mass) * dt * TIME_SCALE;
		nucleons[i].velocity.y += (nucleons[i].force.y / nucleons[i].mass) * dt * TIME_SCALE;

		nucleons[i].position.x += (nucleons[i].velocity.x * dt * TIME_SCALE) / DISTANCE_SCALE;
		nucleons[i].position.y += (nucleons[i].velocity.y * dt * TIME_SCALE) / DISTANCE_SCALE;
	}
}

void CollideWithWalls(void)
{
	for (int i = 0; i < NUM_NUCLEONS; i++)
	{
		if (nucleons[i].position.x < nucleons[i].radius)
		{
			nucleons[i].position.x = nucleons[i].radius;
			nucleons[i].velocity.x = -nucleons[i].velocity.x;
		}
		if (nucleons[i].position.y < nucleons[i].radius)
		{
			nucleons[i].position.y = nucleons[i].radius;
			nucleons[i].velocity.y = -nucleons[i].velocity.y;
		}

		if (nucleons[i].position.x > WIDTH - nucleons[i].radius)
		{
			nucleons[i].position.x = WIDTH - nucleons[i].radius;
			nucleons[i].velocity.x = -nucleons[i].velocity.x;
		}
		if (nucleons[i].position.y > HEIGHT - nucleons[i].radius)
		{
			nucleons[i].position.y = HEIGHT - nucleons[i].radius;
			nucleons[i].velocity.y = -nucleons[i].velocity.y;
		}
	}
}

void DrawNucleons(void)
{
	Color color;
	for (int i = 0; i < NUM_NUCLEONS; i++)
	{
		switch (nucleons[i].charge)
		{
			case CHARGE_NEGATIVE:
				color = RED;
				break;
			
			case CHARGE_NEUTRAL:
				color = BLUE;
				break;
			
			case CHARGE_POSITIVE:
				color = GREEN;
				break;
		}
		DrawCircle((float)nucleons[i].position.x, (float)nucleons[i].position.y, nucleons[i].radius, color);
	}
}