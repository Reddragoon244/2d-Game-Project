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

Entity_t *entity_load(Sprite *sprite, int health, int healthMax)
{
	int i;
	
  /*first search to see if the requested entity sprite image is alreday loaded*/
  for(i = 0; i < NumEntity; i++)
  {
    if(strncmp(sprite->filename,EntityList[i].sprite->filename,20)==0)
    {
      EntityList[i].inuse++;
      return &EntityList[i];
    }
  }

  /*makesure we have the room for a new entity*/
  if(NumEntity + 1 >= entityMax)
  {
        fprintf(stderr, "Maximum Entities Reached.\n");
        
  }

  /*if its not already in memory, then load it.*/
  NumEntity++;
  for(i = 0;i <= NumEntity;i++)
  {
    if(!EntityList[i].inuse)break;
  }

  EntityList[i].sprite = sprite;

  /*initialization of position and velocity */
  EntityList[i].position.x = 0;
  EntityList[i].position.y = 0;
  EntityList[i].velocity.x = 0;
  EntityList[i].velocity.y = 0;

  EntityList[i].health = health;
  EntityList[i].healthMax = healthMax;

  EntityList[i].state = 0;/* initialize the state to 0 */
  EntityList[i].inuse++;/* inuse is used to keep a count of the number of times this sprite is used*/

  return &EntityList[i];

}


void entity_free(Entity_t *entity)
{
  /*first lets check to see if the entity sprite is still being used.*/
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