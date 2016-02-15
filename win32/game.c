#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "sprite.h"
#include "entity.h"
#include "vector.h"
#include <string>

SDL_Renderer *renderer;
SDL_Texture *texture;

int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;//always make a poiter null at initial
	SDL_Surface *temp = NULL;
	SDL_Rect playerRect, playerPos;
	Sprite *spritelist[spriteMax];
	Entity_t *entitylist[entityMax];
	playerPos.x = playerPos.y = 0;//position of image
	playerPos.w = playerPos.h = 64;//size of the image on screen
	int textureW, textureH;
	int frameW, frameH, FPS = 60, frameTime = 0;
	bool isRunning = true;
	Uint8 keys;
	SDL_Event ev;


		window = SDL_CreateWindow("Into the Twilight", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 720, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

		initSpriteSystem();/*The initialization of the sprite system*/
		initEntitySystem();/*The initialization of the entity system*/

		spritelist[0] = sprite_load("image.bmp", 13, 21, renderer);/*Function to load the sprite file into the array Sprite List*/
		entitylist[0] = entity_load(spritelist[0], 1, 1);/*Function to load the sprite into the entitylist */

		SDL_QueryTexture(entitylist[0]->sprite->image, NULL, NULL, &spritelist[0]->imageW, &spritelist[0]->imageH);/*SDL Function to stage the texture*/

			while(isRunning)
			{
				while(SDL_PollEvent(&ev) != 0)
				{
					if(ev.type == SDL_QUIT)
						isRunning = false;
				}

				frameTime++;

				if(frameTime == 1000)
				{
					frameTime = 0;
					//playerRect.x += spritelist[0]->frameW;
					//if(playerRect.x >= spritelist[0]->imageW)
						//playerRect.x = 0;
				}

				SDL_RenderClear(renderer);
				sprite_draw(entitylist[0]->sprite, 273, renderer, NULL, NULL);/*Call the draw to draw the sprite to the screen*/
				SDL_RenderPresent(renderer);
			}
		
	
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	closeSpriteSystem();
	closeEntitySystem();
	window = NULL;
	texture = NULL;
	renderer = NULL;

	SDL_Quit();

	return 0;
}
















