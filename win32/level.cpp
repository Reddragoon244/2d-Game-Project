#include <stdlib.h>
#include <string>
#include "level.h"
#include "Camera.h"

Level *levelList = NULL;
Level level;
Level LevelList[levelMax];/*list of sprites*/
Level *Mlevel;
int NumLevel;

void initPlatformSystem()
{
	levelList = (Level *)malloc(sizeof(Level)*levelMax);
	memset(LevelList, 0,sizeof(Level)*levelMax);
	atexit(closePlatformSystem);
}

void closePlatformSystem()
{
  int i;
   for(i = 0;i < levelMax;i++)
   {
     /*it shouldn't matter if the entity is already freed, 
     entity_free checks for that*/
      platform_free(&LevelList[i]);
   }
}

void platform_free(Level *platform)
{
  /*first lets check to see if the entity sprite is still being used.*/
  platform->inuse--;
  if(platform->inuse == 0)
  {
		strcpy(platform->name,"\0");
		
		if(platform->background != NULL)SDL_DestroyTexture(platform->background);
			platform->background = NULL;
  }
 /*and then lets make sure we don't leave any potential seg faults 
  lying around*/
}

Level *levelloadbg(char *filepath, SDL_Renderer *renderer)
{
	SDL_Texture *texture = NULL;
	SDL_Surface *surface = IMG_Load(filepath);

	if(surface == NULL)
	  fprintf(stderr, "Error surface null", SDL_GetError());
	else
	{
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		if(texture == NULL)
			fprintf(stderr, "Error texture null", SDL_GetError());
	}
	        
	SDL_FreeSurface(surface);

	level.background = texture;

	return &level;
}

Level *levelloadpl(char *filepath, SDL_Renderer *renderer)
{
	int i;

	SDL_Texture *texture = NULL;
	SDL_Surface *surface = IMG_Load(filepath);

  for(i = 0; i < NumLevel; i++)
  {
    if(strncmp(filepath,LevelList[i].name,20)==0)
    {
      LevelList[i].inuse++;
      return &LevelList[i];
    }
  }

  if(NumLevel + 1 >= levelMax)
  {
        fprintf(stderr, "Maximum Entities Reached.\n");
        
  }

	NumLevel++;

  for(i = 0;i <= NumLevel;i++)
  {
    if(!LevelList[i].inuse)break;
  }

	if(surface == NULL)
	  fprintf(stderr, "Error surface null", SDL_GetError());
	else
	{
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		if(texture == NULL)
			fprintf(stderr, "Error texture null", SDL_GetError());
	}
	        
	SDL_FreeSurface(surface);

	SDL_QueryTexture(texture, NULL, NULL, &LevelList[i].Width, &LevelList[i].Height);

	LevelList[i].platform = texture;

	return &LevelList[i];
}

void levelDraw(Level *level, SDL_Renderer *renderer, SDL_Rect &Camera)
{	
	SDL_RenderCopy(renderer, level->background, &Camera, NULL);
}

void levelPlatformDraw(Level *level, SDL_Renderer *renderer, SDL_Rect &Camera, int drawX, int drawY)
{	
	SDL_Rect src,dest;

	src.x = 0;
	src.y = 0;
    src.w = level->Width;
    src.h = level->Height;
    dest.x = drawX - Camera.x;
    dest.y = drawY - Camera.y;
    dest.w = level->Width;
    dest.h = level->Height;

	SDL_RenderCopy(renderer, level->platform, &src, &dest);
}

void levelFree(Level **level)
{

}
