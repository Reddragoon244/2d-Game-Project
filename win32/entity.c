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

Entity_t *entity_load(Sprite *sprite, float health, float healthMax)
{
	int i;
	
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

  EntityList[i].origin.x = EntityList[i].sprite->frameW/2;
  EntityList[i].origin.y = EntityList[i].sprite->frameH/2;

  //EntityList[i].bounds.w = EntityList[i].sprite->frameW;
  //EntityList[i].bounds.h = EntityList[i].sprite->frameH;
  //EntityList[i].bounds.x = 0;
  //EntityList[i].bounds.y = 0;

  EntityList[i].groundBounds.w = EntityList[i].sprite->frameW - 2;
  EntityList[i].groundBounds.h = 5;
  EntityList[i].groundBounds.x = 1;
  EntityList[i].groundBounds.y = EntityList[i].sprite->frameW;

  EntityList[i].EventBounds.w = 0;
  EntityList[i].EventBounds.h = 0;
  EntityList[i].EventBounds.x = 0;
  EntityList[i].EventBounds.y = 0;

  EntityList[i].PositionRect.w = 0;
  EntityList[i].PositionRect.h = 0;
  EntityList[i].PositionRect.x = 0;
  EntityList[i].PositionRect.y = 0;

  EntityList[i].PositionTemp.w = 0;
  EntityList[i].PositionTemp.h = 0;
  EntityList[i].PositionTemp.x = 0;
  EntityList[i].PositionTemp.y = 0;

  EntityList[i].drawn = 0;

  EntityList[i].health = health;
  EntityList[i].healthMax = healthMax;

  EntityList[i].think;

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
     /*it shouldn't matter if the entity is already freed, 
     entity_free checks for that*/
      entity_free(&EntityList[i]);
   }
}

int entity_intersect_rect(Entity_t *a,SDL_Rect r)
{
    SDL_Rect aB;
    if (!a)
    {
        return 0;
    }
    aB = rect(
        a->position.x + a->bounds.x,
        a->position.y + a->bounds.y,
        a->bounds.w,
        a->bounds.h);
    return rect_intersect(aB,r);
}

int entity_intersect_all(Entity_t *a)
{
    int i;
	SDL_Rect zero = {0,0,0,0};

    if (!a)return NULL;

    for (i = 0; i < entityMax;i++)
    {
        if (!EntityList[i].inuse)
        {
            continue;
        }
        if (a == &EntityList[i])
        {
            /*don't clip self*/
        }
		else if(entity_intersect(a, &EntityList[i]))
		{
			printf("%s \n", EntityList[i].sprite->filename);
				return 1;
        }
    }
    return 0;
}

int entity_intersect_event_all(Entity_t *a)
{
	int i;
	SDL_Rect zero = {0,0,0,0};

    if (!a)return NULL;

    for (i = 0; i < entityMax;i++)
    {
        if (!EntityList[i].inuse)
        {
            continue;
        }
        if (a == &EntityList[i])
        {
            /*don't clip self*/
        }
		else if(entity_intersect_event(a, &EntityList[i]))
		{
			printf("%s \n", EntityList[i].sprite->filename);
				return 1;
        }
    }
    return 0;
}

Entity_t *entity_return_intersect_all(Entity_t *a)
{
    int i;
	SDL_Rect zero = {0,0,0,0};

    if (!a)return NULL;

    for (i = 0; i < entityMax;i++)
    {
        if (!EntityList[i].inuse)
        {
            continue;
        }
        if (a == &EntityList[i])
        {
            /*don't clip self*/
        }
		else if(entity_intersect(a, &EntityList[i]))
		{
				return &EntityList[i];
        }
    }
    return NULL;
}

int entity_ground_intersect_all(Entity_t *a)
{
    int i;
	SDL_Rect zero = {0,0,0,0};

    if (!a)return NULL;

    for (i = 0; i < entityMax;i++)
    {
        if (!EntityList[i].inuse)
        {
            continue;
        }
        if (a == &EntityList[i])
        {
            /*don't clip self*/
        }
		else if(entity_ground_intersect(a, &EntityList[i]))
		{
				return 1;
        }
    }
    return 0;
}

int entity_intersect(Entity_t *a, Entity_t *b)
{
	SDL_Rect aB, bB;

	if((!a) && (!b))
	{
		return 0;
	}

	if(a->drawn == 0 || b->drawn == 0)
	{
		return 0;
	}

	aB = rect(a->position.x + a->bounds.x,
			a->position.y + a->bounds.y,
			a->bounds.w,
			a->bounds.h);
	bB = rect(b->position.x + b->bounds.x,
        b->position.y + b->bounds.y,
        b->bounds.w,
        b->bounds.h);
    return rect_intersect(aB,bB);
}

int entity_intersect_event(Entity_t *a, Entity_t *b)
{
	SDL_Rect aB, bB;

	if((!a) && (!b))
	{
		return 0;
	}

	if(a->drawn == 0 || b->drawn == 0)
	{
		return 0;
	}

	aB = rect(a->position.x + a->bounds.x,
			a->position.y + a->bounds.y,
			a->bounds.w,
			a->bounds.h);
	bB = rect(b->position.x + b->EventBounds.x,
        b->position.y + b->EventBounds.y,
        b->EventBounds.w,
        b->EventBounds.h);
    return rect_intersect(aB,bB);
}

int entity_ground_intersect(Entity_t *a, Entity_t *b)/*First entity has to be the entity you want to ground check*/
{
	SDL_Rect aB, bB;

	if((!a) && (!b))
	{
		return 0;
	}

	if(a->drawn == 0 || b->drawn == 0)
	{
		return 0;
	}

	aB = rect(a->position.x + a->groundBounds.x,
			a->position.y + a->groundBounds.y,
			a->groundBounds.w,
			a->groundBounds.h);
	bB = rect(b->position.x + b->bounds.x,
        b->position.y + b->bounds.y,
        b->bounds.w,
        b->bounds.h);
    return rect_intersect(aB,bB);
}

void entity_draw(Entity_t *entity, int frame, SDL_Renderer *renderer, int drawX, int drawY, SDL_Rect &Camera)
{
	SDL_Rect src,dest;

	src.x = (frame%entity->sprite->fpl) * entity->sprite->frameW;//frame%sprite->fpl * sprite->frameW;
	src.y = (frame/entity->sprite->fpl) * entity->sprite->frameH;//frame/sprite->fpl * sprite->frameH;
    src.w = entity->sprite->frameW;
    src.h = entity->sprite->frameH;
    entity->position.x = dest.x = drawX - Camera.x;
	entity->position.y = dest.y = drawY - Camera.y;
    dest.w = entity->sprite->frameW;
    dest.h = entity->sprite->frameH;

	entity->bounds.w = entity->sprite->frameW;
    entity->bounds.h = entity->sprite->frameH;
	entity->bounds.x = 0;
	entity->bounds.y = 0;

	entity->drawn = 1;

	SDL_RenderCopy(renderer, entity->sprite->image, &src, &dest);
}

Entity_t *duplicEntity(Entity_t *a)
{
	int i;

	for(i = 0; i<NumEntity; i++)
	{
	  if(!EntityList[i].sprite->filename)
	  {
		  EntityList[i] = *a;
		  break;
	  }
	}


	return &EntityList[i];
}

/*

Entity_S *entity_new()
{
	int i;
	for(i=0;i<entityMax;i++)
	{
		if(entitylist[i].inuse)
		{
		continue;
		}

		memset(&entitylist[i],0,sizeof(Entity_S));

		entitylist[i].inuse = 1;
		return &entityList[i];
	}

	return NULL;

}


void entity_free(Entity_t **entity)
{
	Entity_t *self;
	if(!entity)return;
	if(!*entity)return;

	self = *entity;
	freesprite(&self->sprite);
	*entity = NULL;
}

void entity_thnk_all()
{
	int i;
	for(i=0,i<entityMax;i++)
	{
		if(!entitylist[i].inuse)
		{
			continue;
			}

			if(!entitylist[i].think)
			{
			continue;
			}

			entitylist[i].think(&entitylist[i]);
	}

}

void entity_update_all()
{
	int i;
	for(i=0,i<entityMax;i++)
	{
		if(!entitylist[i].inuse)
		{
			continue;
			}

			Vector2dAdd(entitylist[i].position,entitylist[i].velocity,entitylist[i].position);

			if(!entitylist[i].update)
			{
			continue;
			}

			entitylist[i].update(&entitylist[i]);
	}

}



*/