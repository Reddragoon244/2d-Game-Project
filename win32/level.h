#ifndef _LEVEL_H_
#define _LEVEL_H_

#include<SDL.h>
#include<SDL_image.h>
#include "sprite.h"
#include "vector.h"

typedef struct{

	Sprite *sprite;
	int frame;
	int soild;

}tile;

typedef struct{

	char name[128];
	Sprite *tileSet;//dont need struct tile
	int *tiles;
	int tileWidth;
	int tileHeight;
	int tileCount;
	Vector2d tileSize;
	SDL_Texture *background;
	SDL_Texture *platform;

}Level;

/**
  * @brief
  * @param
  * @return
  */

Level *levelload(char *filepath, SDL_Renderer *renderer);
void levelDraw(Level *level, SDL_Renderer *renderer);
void levelFree(Level **level);

#endif