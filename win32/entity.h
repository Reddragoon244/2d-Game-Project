#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "sprite.h"
#include "vector.h"

/**
	*@brief a simple entity structure
	*/

typedef struct Entity_S
{
	int inuse;
	Sprite_T *sprite;
	int frame;
	Vector2d position;
	Vector2d velocity;
	float health, healthMax;
	int state;

	void (*think)(struct Entity_S *self);

}Entity_t

/* 

initialwholesystem
draw call draw sprite
new
delete
update

*/

#endif