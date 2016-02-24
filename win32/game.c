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
	int FPS = 60, frameTime = 0, frame = 0;//frame is the frame of the sprite sheet
	int isRunning = 1;
	Uint8 keys;
	SDL_Event ev;


		window = SDL_CreateWindow("Into the Twilight", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 720, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

		initSpriteSystem();/*The initialization of the sprite system*/
		initEntitySystem();/*The initialization of the entity system*/

		spritelist[0] = sprite_load("bgtest.png", 1, 1, renderer);/*Function to load the sprite file into the array Sprite List*/
		entitylist[0] = entity_load(spritelist[0], NULL, NULL);/*Function to load the sprite into the entitylist */

		spritelist[1] = sprite_load("image.bmp", 13, 21, renderer);/*Function to load the sprite file into the array Sprite List*/
		entitylist[1] = entity_load(spritelist[1], 1, 1);/*Function to load the sprite into the entitylist */

			while(isRunning)
			{
				while(SDL_PollEvent(&ev) != 0)
				{
					if(ev.type == SDL_QUIT)
						isRunning = 0;
				}

				frameTime++;

				if(frameTime == 500)
				{
					if(frame < 144)
						frame = 144;

					if(frame > 150)
						frame = 144;
					else
						frame++;

					frameTime = 0;
				}

				SDL_RenderClear(renderer);
				sprite_draw(entitylist[0]->sprite, 1, renderer, NULL, NULL);/*Call the draw to draw the sprite to the screen*/
				sprite_draw(entitylist[1]->sprite, frame, renderer, playerPos.x, playerPos.y);/*Call the draw to draw the sprite to the screen*/
				SDL_RenderPresent(renderer);
			}
		
	
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	window = NULL;
	texture = NULL;
	renderer = NULL;

	SDL_Quit();

	return 0;
}
















