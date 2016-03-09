#ifndef _LEVEL_H_
#define _LEVEL_H_

#include<SDL.h>
#include<SDL_image.h>
#include "sprite.h"
#include "vector.h"

static const Uint32 levelMax = 1000;

typedef struct{

	Sprite *sprite;
	int frame;
	int soild;

}tile;

typedef struct{

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

void initPlatformSystem();
void closePlatformSystem();
void platform_free(Level *platform);
Level *levelloadbg(char *filepath, SDL_Renderer *renderer);
Level *levelloadpl(char *filepath, SDL_Renderer *renderer);
void levelDraw(Level *level, SDL_Renderer *renderer, SDL_Rect &Camera);
void levelPlatformDraw(Level *level, SDL_Renderer *renderer, SDL_Rect &Camera, int drawX, int drawY);
void levelFree(Level **level);

#endif