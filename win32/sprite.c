#include <string>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "sprite.h"


Sprite *spriteList = NULL;


SDL_Window *window; /*pointer to the window handler */
SDL_Surface *buffer; /*pointer to the background image buffer*/
SDL_Surface *videobuffer; /*pointer to the draw buffer*/
SDL_Rect Camera; /*x & y are the coordinates for the background map, w and h are of the screen*/
Sprite SpriteList[spriteMax];/*list of sprites*/
Sprite *Msprite;
int NumSprites;

/**
	*@brief Initialization of the sprite system
	*/

void initSpriteSystem()
{
	spriteList = (Sprite *)malloc(sizeof(Sprite)*spriteMax);
	memset(spriteList, 0,sizeof(Sprite)*spriteMax);
	atexit(closeSpriteSystem);
}

Sprite *sprite_load(char *filename,int sizex, int sizey, SDL_Renderer *renderer)
{
	int i;
  SDL_Texture *temp;

  /*first search to see if the requested sprite image is alreday loaded*/
  for(i = 0; i < NumSprites; i++)
  {
    if(strncmp(filename,SpriteList[i].filename,20)==0)
    {
      SpriteList[i].refcount++;
      return &SpriteList[i];
    }
  }

  /*makesure we have the room for a new sprite*/
  if(NumSprites + 1 >= spriteMax)
  {
        fprintf(stderr, "Maximum Sprites Reached.\n");
        
  }
  /*if its not already in memory, then load it.*/
  NumSprites++;
  for(i = 0;i <= NumSprites;i++)
  {
    if(!SpriteList[i].refcount)break;
  }

  SDL_Surface *surface = SDL_LoadBMP(filename);
  temp = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  if(temp == NULL)
  {
    fprintf(stderr,"unable to load a vital sprite: %s\n",SDL_GetError());
    
  }

  SpriteList[i].image = temp;/*This is the sprite's texture */

  strcpy(SpriteList[i].filename, filename);/*Copy filname into Sprite filename*/

  SpriteList[i].fpl = sizex;/* Make every spritesheet 13 frames per line or this needs to change*/
  SpriteList[i].imageW = sizex;/* Texture Size of the frames so a 4x3 sprite sheet this would be 4*/
  SpriteList[i].imageH = sizey;/* Texture Size of the frames so a 4x3 sprite sheet this would be 3*/
  SpriteList[i].refcount++;/* refcount is used to keep a count of the number of times this sprite is used*/

  SDL_QueryTexture(SpriteList[i].image, NULL, NULL, &SpriteList[i].imageW, &SpriteList[i].imageH);/*SDL Function to stage the texture*/

  SpriteList[i].frameW = SpriteList[i].imageW / sizex;
  SpriteList[i].frameH = SpriteList[i].imageH / sizey;

  return &SpriteList[i];

}

void sprite_free(Sprite *sprite)
{
  /*first lets check to see if the sprite is still being used.*/
  sprite->refcount--;
  if(sprite->refcount == 0)
  {
		strcpy(sprite->filename,"\0");
		
		if(sprite->image != NULL)SDL_DestroyTexture(sprite->image);
			sprite->image = NULL;
  }
 /*and then lets make sure we don't leave any potential seg faults 
  lying around*/
}

void closeSpriteSystem()
{
  int i;
   for(i = 0;i < spriteMax;i++)
   {
     /*it shouldn't matter if the sprite is already freed, 
     sprite_free checks for that*/
      sprite_free(&SpriteList[i]);
   }
}

void sprite_draw(Sprite *sprite, int frame, SDL_Renderer *renderer, int drawX, int drawY, int scale, SDL_Rect &Camera)
{
	SDL_Rect src,dest;

	src.x = (frame%sprite->fpl) * sprite->frameW;//frame%sprite->fpl * sprite->frameW;
	src.y = (frame/sprite->fpl) * sprite->frameH;//frame/sprite->fpl * sprite->frameH;
    src.w = sprite->frameW;
    src.h = sprite->frameH;
    dest.x = drawX - Camera.x;
    dest.y = drawY - Camera.y;
    dest.w = sprite->frameW * scale;
    dest.h = sprite->frameH * scale;
	/* Need a SDL function texture to renderer*/
	SDL_RenderCopy(renderer, sprite->image, &src, &dest);

}