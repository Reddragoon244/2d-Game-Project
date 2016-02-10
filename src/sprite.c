#include <string>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "sprite.h"


Sprite *spriteList = NULL;
const int spriteMax = 1000;

SDL_Window *window; /*pointer to the window handler */
SDL_Surface *buffer; /*pointer to the background image buffer*/
SDL_Surface *videobuffer; /*pointer to the draw buffer*/
SDL_Rect Camera; /*x & y are the coordinates for the background map, w and h are of the screen*/
Sprite SpriteList[spriteMax];/*list of sprites*/
Sprite *Msprite;
int NumSprites;

void initSpriteSystem()
{
	spriteList = (Sprite *)malloc(sizeof(Sprite)*spriteMax);
	memset(spriteList, 0,sizeof(Sprite)*spriteMax);
	atexit(closeSpriteSystem);
}

Sprite *sprite_load(char *filename,int sizex, int sizey)
{
	int i;
  SDL_Surface *temp;
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
        exit(1);
  }
  /*if its not already in memory, then load it.*/
  NumSprites++;
  for(i = 0;i <= NumSprites;i++)
  {
    if(!SpriteList[i].refcount)break;
  }
  temp = IMG_Load(filename);

  if(temp == NULL)
  {
    fprintf(stderr,"unable to load a vital sprite: %s\n",SDL_GetError());
    exit(0);
  }

  SpriteList[i].image = temp;

  SpriteList[i].fpl = 16;
  SpriteList[i].imageW = sizex;
  SpriteList[i].imageH = sizey;
  SpriteList[i].refcount++;

  return &SpriteList[i];

}

void sprite_free(Sprite *sprite)
{
  /*first lets check to see if the sprite is still being used.*/
  sprite->refcount--;
  if(sprite->refcount == 0)
  {
		strcpy(sprite->filename,"\0");
		
		if(sprite->image != NULL)SDL_FreeSurface(sprite->image);
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

void sprite_draw(Sprite *sprite, int frame, SDL_Surface *surface, int drawX, int drawY)
{
    SDL_Rect src,dest;
    src.x = frame%sprite->fpl * sprite->imageW;
    src.y = frame/sprite->fpl * sprite->imageH;
    src.w = sprite->imageW;
    src.h = sprite->imageH;
    dest.x = drawX;
    dest.y = drawY;
    dest.w = sprite->imageW;
    dest.h = sprite->imageH;
    SDL_BlitSurface(sprite->image, &src, surface, &dest); /*find something to surface copy to a destination surface*/
  
}