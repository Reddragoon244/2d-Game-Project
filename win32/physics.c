#include "physics.h"

void moveRight(SDL_Rect &r, SDL_Rect &t, float deltaTime, Entity_t *a, int moveSpeed)
{
	if(entity_intersect_all(a))
	{
		/*Collision with anything*/
		r.x = t.x;
	}
	else
	{
		/*Not Colliding*/
		t.x = r.x;

		r.x += moveSpeed * deltaTime;
	}
}

void moveLeft(SDL_Rect &r, SDL_Rect &t, float deltaTime, Entity_t *a, int moveSpeed)
{
	if(entity_intersect_all(a))
	{
		/*Collision with anything*/
		r.x = t.x;
	}
	else
	{
		/*Not Colliding*/
		t.x = r.x;

		r.x -= moveSpeed * deltaTime;
	}
}

void Jump(SDL_Rect &r, SDL_Rect &t, float deltaTime, Entity_t *a, int moveSpeed)
{
	if(entity_intersect_all(a))
	{
		/*Collision with anything*/
		r.y = t.y;
	}
	else
	{
		/*Not Colliding*/
		t.y = r.y;

		r.y -= moveSpeed * deltaTime;
	}
}

void Fall(SDL_Rect &r, SDL_Rect &t, float deltaTime, Entity_t *a, int moveSpeed)
{
	if(entity_intersect_all(a))
	{
		/*Collision with anything*/
		r.y = t.y;
							

	}
	else if(!entity_ground_intersect_all(a))
	{
		/*Not Colliding*/
		t.y = r.y;
														
			if(r.y < 656)
				r.y += 1 * deltaTime;				
	}
}