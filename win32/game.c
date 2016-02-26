#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "sprite.h"
#include "entity.h"
#include "vector.h"
#include "level.h"
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
	Level *backg;
	playerPos.x = 0;//position of image.x
	playerPos.y = 0;//position of image.y
	int FPS = 60, frame = 0;//frame is the frame of the sprite sheet
	int isRunning = 1;
	const Uint8 *keys;
	SDL_Event ev;
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	int currentTime = 0;
	int previousTime = 0;
	float frameTime = 0;
	float deltaTime = 0;
	float moveSpeed = 1.0;
	int jumpCheck = 1;
	int jump = 0;
	int jumpMax = 200;

		window = SDL_CreateWindow("Into the Twilight", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 720, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);	
		
		if(!(IMG_Init(imgFlags) & imgFlags))
			fprintf(stderr, "Error INIT PNG or JPG", SDL_GetError());

		initSpriteSystem();/*The initialization of the sprite system*/
		initEntitySystem();/*The initialization of the entity system*/

		backg = levelload("bgtest.png", renderer);

		spritelist[1] = sprite_load("image.bmp", 13, 21, renderer);/*Function to load the sprite file into the array Sprite List*/
		entitylist[1] = entity_load(spritelist[1], 1, 1);/*Function to load the sprite into the entitylist */

		frame = 143;/*starting frame*/
		playerPos.y = 656;/*in world space where the player starts on the y axis*/

			while(isRunning)
			{
				previousTime = currentTime;
				currentTime = SDL_GetTicks() / 3;
				deltaTime = (currentTime - previousTime);

				while(SDL_PollEvent(&ev) != 0)
				{
					if(ev.type == SDL_QUIT)
						isRunning = 0;
				}

				keys = SDL_GetKeyboardState(NULL);

				if(keys[SDL_SCANCODE_D])/*MOVE RIGHT*/
				{	
					playerPos.x += moveSpeed * deltaTime;
					frameTime += deltaTime;

					if(frameTime >= 12.5f)
					{
						if(frame < 143)
							frame = 143;

						if(frame > 150)
							frame = 143;
					else
							frame++;

						frameTime = 0;
					}
				}

				if(keys[SDL_SCANCODE_A])/*MOVE LEFT*/
				{	
					playerPos.x -= moveSpeed * deltaTime;				
					frameTime += deltaTime;

					if(frameTime >= 12.5f)
					{
						if(frame < 117)
							frame = 117;

						if(frame > 123)
							frame = 117;
					else
							frame++;

						frameTime = 0;
					}
				}

				if(keys[SDL_SCANCODE_SPACE] && jumpCheck == 1)/*Jumping*/
				{
					if(jump < jumpMax)
					{
						jump += moveSpeed * deltaTime;

						fprintf(stderr, "%i ", jump);
						fprintf(stderr, "%i ", jumpCheck);

						playerPos.y -= moveSpeed * deltaTime;				
						frameTime += deltaTime;
					}
					else 
						jumpCheck = 0;

				}

				else if(playerPos.y <= 656 && jumpCheck == 0)/*Falling, need to fix*/
				{	
					if(jump > 0)
					{
						jump -= moveSpeed * deltaTime;

						playerPos.y += moveSpeed * deltaTime;				
						frameTime += deltaTime;
					}
					else
						jumpCheck = 1;
				}

				SDL_RenderClear(renderer);
				levelDraw(backg, renderer);
				sprite_draw(entitylist[1]->sprite, frame, renderer, playerPos.x, playerPos.y);/*Call the draw to draw the sprite to the screen*/
				SDL_RenderPresent(renderer);
			}
		
	
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	window = NULL;
	texture = NULL;
	renderer = NULL;

	IMG_Quit();
	SDL_Quit();

	return 0;
}
















