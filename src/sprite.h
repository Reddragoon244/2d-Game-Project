#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

static const int spriteMax = 1000;

typedef struct Sprite_T
{
	int refcount;
	char filename[128];
	SDL_Texture *image;
	int imageW, imageH;
	int frameW, frameH;
	int fpl;

}Sprite;


/**
  * @brief
  * @param 
  */

void initSpriteSystem();/*clear and initial sprites*/
void closeSpriteSystem();/*call this before you exit to make sure all the memory that your program used is given back to your OS*/

Sprite *sprite_load(char *filename,int sizex, int sizey, SDL_Renderer *renderer);/*simplestway to load a file*/
void sprite_free(Sprite *sprite);
void sprite_draw(Sprite *sprite, int frame, SDL_Surface *surface, int drawX, int drawY);


/*
Vector 2D, 3D, 4D

float x,y,z,w
add, multiply, cross poduct
set, negate, scale, rotate, normalized
getLength, GetLengthSquared
getAngle, getAngle to vector

*/

#endif 