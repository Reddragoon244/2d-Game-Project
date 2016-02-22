#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <math.h>
#include "vector.h"

/*Function to compare two vectors */
int VectorCompare(Vector2d v1, Vector2d v2)
{
	if(v1.x != v2.x || v1.y != v2.y)
		return 0;

	return 1;
}

/*Function to normalize a 2d vector */
Vector2d Normalize(Vector2d v)
{
	float i;
	Vector2d length;

	length.x = v.x*v.x;
	length.y = v.y*v.y;
	length.x = sqrt (length.x);
	length.y = sqrt (length.y);

	if(length.x)
	{
		i = 1/length.x;
		v.x *= i;
	}

	if(length.y)
	{
		i = 1/length.y;
		v.y *= i;
	}

	return length;
}

/*Function to scale a 2d vector */
void Scale(Vector2d v1, int scale, Vector2d v2)
{
	v2.x = v1.x*scale;
	v2.y = v1.y*scale;
}

int rect_intersect(SDL_Rect a, SDL_Rect b)
{
	if((a.x + a.w >= b.x) &&
		(b.x + b.w >= a.x) &&
		(a.y + a.h >= b.y) &&
		(b.y + b.h >= a.y))
	{
		return 1;
	}
	else
		return 0;
}

