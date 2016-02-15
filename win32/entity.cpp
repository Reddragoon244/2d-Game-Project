#include <string>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "entity.h"

Entity_t *entityList = NULL;

Entity_t EntityList[entityMax];/*list of sprites*/
Entity_t *Mentity;
int NumEntity;

void initEntitySystem()
{
	entityList = (Entity_t *)malloc(sizeof(Entity_t)*entityMax);
	memset(EntityList, 0,sizeof(Entity_t)*entityMax);
	atexit(closeEntitySystem);
}

void entity_free(Entity_t *entity)
{
  /*first lets check to see if the sprite is still being used.*/
  entity->inuse--;
  if(entity->inuse == 0)
  {
		strcpy(entity->sprite->filename,"\0");
		
		if(entity->sprite->image != NULL)SDL_DestroyTexture(entity->sprite->image);
			entity->sprite->image = NULL;
  }
 /*and then lets make sure we don't leave any potential seg faults 
  lying around*/
}

void closeEntitySystem()
{
  int i;
   for(i = 0;i < entityMax;i++)
   {
     /*it shouldn't matter if the sprite is already freed, 
     sprite_free checks for that*/
      entity_free(&EntityList[i]);
   }
}