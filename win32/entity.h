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

static const Uint32 entityMax = 1000;

typedef struct Entity_S
{
	int inuse;						/**<flag for tracking resource use*/
	Sprite_T *sprite;
	int frame;
	int cameraEnt;
	Vector2d position;
	Vector2d velocity;
	SDL_Rect bounds;
	float health, healthMax;
	int state;

	void (*draw) (struct Entity_S *self, SDL_Renderer *renderer);
	void (*think)(struct Entity_S *self);/**<think function for entity*/
	int nextThink;/**<time index for next think*/
	int thinkRate;/**<how often to run think*/
	void (*update) (struct Entity_S *self);/**<update the entity*/
	void (*touch) (struct Entity_S *self, struct Entity_S *other);
	void (*free) (struct Entity_S *self);/**<cleanup function called on free*/

}Entity_t;

/**
*@brief Initialization for entity systems
*
*/

void initEntitySystem();

/**
*@brief free a entity from the memory
*
*/

void entity_free(Entity_t **entity);

/**
*@brief return a pointer to an empty entity struct
*return NULL on error or no more space for entities
*/
Entity_t *entity_load(Sprite *sprite, float health, float healthMax);

void closeEntitySystem();

void entity_think_all();
void entity_update_all();
void entity_draw_all();

/* 

initialwholesystem
draw call draw sprite
new
delete
update

*/

#endif