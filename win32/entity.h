#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "sprite.h"


typedef struct Entity_S
{
	int inuse;
	Entity_t *EList;
	Sprite_T *sprite;
	int frame;
	int x, y;
	int health, healthMax;
	int velX, velY;
	int state;

	void (*think)(struct Entity_S *self);

}Entity_t

#endif