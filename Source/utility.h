#pragma once
#include <cmath>

#include "raylib.h"

float random_float_01()
{
	return rand() / (float)(RAND_MAX);
}
 Vector2 random_direction()
{
	float angle = random_float_01() * 2 * PI;
	return Vector2{ cosf(angle),sinf(angle) };
}

Vector2 Random_Vector()
{
	Vector2 random_pos;
	random_pos.x = (float)GetRandomValue(-300, 300);
	random_pos.y = (float)GetRandomValue(-300, 300);

	return random_pos;
}

bool OutofScreen(Vector2 position, float offset)
{
	return position.x - offset< 0 || position.x + offset > GetRenderWidth() || position.y - offset < 0 || position.y + offset > GetRenderHeight();
}


Vector2 GetRandomAdjacentTile()
{
	return{(float)GetRandomValue( - 1,1), (float)GetRandomValue (- 1, 1) };
}
