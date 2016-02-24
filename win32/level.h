#ifndef _LEVEL_H_
#define _LEVEL_H_

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
	Sprite *background;

}level;

/**
  * @brief
  * @param
  * @return
  */

level *levelload(char *filepath);
void levelDraw(level *level);
void levelFree(level **level);

#endif