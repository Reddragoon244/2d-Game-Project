#include <stdlib.h>
#include <string>
#include "level.h"
#include "Camera.h"

Level *levelList = NULL;
Level level;
Level LevelList[levelMax];/*list of sprites*/
Level *Mlevel;
int NumLevel;

void initLevelSystem()
{
	levelList = (Level *)malloc(sizeof(Level)*levelMax);
	memset(LevelList, 0,sizeof(Level)*levelMax);
	atexit(closeLevelSystem);
}

void closeLevelSystem()
{
  int i;
   for(i = 0;i < levelMax;i++)
   {
     /*it shouldn't matter if the entity is already freed, 
     entity_free checks for that*/
      level_free(&LevelList[i]);
   }
}

void level_free(Level *level)
{
  /*first lets check to see if the entity sprite is still being used.*/
  level->inuse--;
  if(level->inuse == 0)
  {
		strcpy(level->name,"\0");
		
		if(level->background != NULL)SDL_DestroyTexture(level->background);
			level->background = NULL;
  }
 /*and then lets make sure we don't leave any potential seg faults 
  lying around*/
}

Level *levelloadbg(char *filepath, SDL_Renderer *renderer)
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

	LevelList[i].background = texture;

	return &LevelList[i];
}

void levelDraw(Level *level, SDL_Renderer *renderer, SDL_Rect &Camera)
{	
	SDL_RenderCopy(renderer, level->background, &Camera, NULL);
}

void levelFree(Level **level)
{

}
