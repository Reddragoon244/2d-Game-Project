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

static const int entityMax = 1000;

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

}Entity_t;

void initEntitySystem();
void entity_free(Entity_t *entity);

Entity_t *entity_load(Sprite *sprite, int health, int healthMax);/*simplest way to store a entity*/
void closeEntitySystem();

/* 

initialwholesystem
draw call draw sprite
new
delete
update

*/

#endif