#ifndef _LEVEL_H_
#define _LEVEL_H_

#include<SDL.h>
#include<SDL_image.h>
#include "entity.h"
#include "vector.h"

static const Uint32 levelMax = 1000;

typedef struct{

	Sprite *sprite;
	int frame;
	int soild;

}tile;

typedef struct Level_T{

	char name[128];
	int inuse;
	int *tiles;
	int Width;
	int Height;
	int tileCount;
	SDL_Texture *background;
	SDL_Texture *platform;

}Level;

/**
  * @brief
  * @param
  * @return
  */

void initLevelSystem();
void closeLevelSystem();
void level_free(Level *level);
Level *levelloadbg(char *filepath, SDL_Renderer *renderer);
void levelDraw(Level *level, SDL_Renderer *renderer, SDL_Rect &Camera);
void levelPlatformDraw(Level *level, SDL_Renderer *renderer, SDL_Rect &Camera, int drawX, int drawY);
void levelChange(Level *level);
void levelFree(Level **level);

#endif