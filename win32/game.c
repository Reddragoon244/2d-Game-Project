#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "sprite.h"
#include "entity.h"
#include "vector.h"
#include "level.h"
#include "Camera.h"
#include <string>

SDL_Renderer *renderer;
SDL_Texture *texture;

int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;//always make a pointer null at initial
	SDL_Surface *temp = NULL;
	SDL_Rect playerRect, playerPos;
	Sprite *spritelist[spriteMax];
	Entity_t *entitylist[entityMax];
	Level *backg[levelMax];
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
	int transCheck = 0;

		window = SDL_CreateWindow("Into the Twilight", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 720, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);	
		
		if(!(IMG_Init(imgFlags) & imgFlags))
			fprintf(stderr, "Error INIT PNG or JPG", SDL_GetError());

		initSpriteSystem();/*The initialization of the sprite system*/
		initEntitySystem();/*The initialization of the entity system*/

		backg[0] = levelloadbg("background.png", renderer);
	    backg[1] = levelloadbg("background2.png", renderer);

		spritelist[0] = sprite_load("image.bmp", 13, 21, renderer);/*Function to load the sprite file into the array Sprite List*/
		spritelist[1] = sprite_load("Hplatform1080.bmp", 1, 1, renderer);/*Platform Sprite Load*/
		spritelist[2] = sprite_load("Hplatform540.bmp",1, 1, renderer);/*Platform Sprite Load*/
		spritelist[3] = sprite_load("Hplatform270.bmp",1, 1, renderer);/*Platform Sprite Load*/
		spritelist[4] = sprite_load("Vplatform1080.bmp",1, 1, renderer);/*Platform Sprite Load*/
		spritelist[5] = sprite_load("Vplatform540.bmp",1, 1, renderer);/*Platform Sprite Load*/
		spritelist[6] = sprite_load("Vplatform270.bmp",1, 1, renderer);/*Platform Sprite Load*/
		spritelist[7] = sprite_load("enemy1.bmp", 3, 1, renderer);/*Function to load the sprite file into the array Sprite List*/
		spritelist[8] = sprite_load("enemy2.bmp", 3, 1, renderer);/*Function to load the sprite file into the array Sprite List*/
		spritelist[9] = sprite_load("lever.bmp", 2, 1, renderer);/*Function to load the sprite file into the array Sprite List*/
		spritelist[10] = sprite_load("box.bmp", 1, 1, renderer);/*Function to load the sprite file into the array Sprite List*/


		entitylist[0] = entity_load(spritelist[0], 1, 1);/*Function to load the sprite into the entitylist */
		entitylist[1] = entity_load(spritelist[1], 1, 1);/*Function to load the sprite into the entitylist */
		entitylist[2] = entity_load(spritelist[2], 1, 1);/*Function to load the sprite into the entitylist */
		entitylist[3] = entity_load(spritelist[3], 1, 1);/*Function to load the sprite into the entitylist */
		entitylist[4] = entity_load(spritelist[4], 1, 1);/*Function to load the sprite into the entitylist */
		entitylist[5] = entity_load(spritelist[5], 1, 1);/*Function to load the sprite into the entitylist */
		entitylist[6] = entity_load(spritelist[6], 1, 1);/*Function to load the sprite into the entitylist */
		entitylist[7] = entity_load(spritelist[7], 1, 1);/*Function to load the sprite into the entitylist */
		entitylist[8] = entity_load(spritelist[8], 1, 1);/*Function to load the sprite into the entitylist */
		entitylist[9] = entity_load(spritelist[9], 1, 1);/*Function to load the sprite into the entitylist */
		entitylist[10] = entity_load(spritelist[10], 1, 1);/*Function to load the sprite into the entitylist */

		frame = 143;/*starting frame*/
		playerPos.y = 656;/*in world space where the player starts on the y axis*/
		backg[2] = backg[0];

			while(isRunning)
			{
				previousTime = currentTime;
				currentTime = SDL_GetTicks() / 4;
				deltaTime = (currentTime - previousTime);

				while(SDL_PollEvent(&ev) != 0)
				{
					if(ev.type == SDL_QUIT)
						isRunning = 0;
				}

				keys = SDL_GetKeyboardState(NULL);

				if(keys[SDL_SCANCODE_D] && !keys[SDL_SCANCODE_A])/*MOVE RIGHT*/
				{	
					if(entity_intersect_all(entitylist[0]))
					{
						/*Collision with anything*/
					}
					else 
					{
						/*Not Colliding*/
					}
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
				
				if(keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D])/*MOVE LEFT*/
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

						playerPos.y -= moveSpeed * deltaTime;				
						frameTime += deltaTime;
					}
					else 
						jumpCheck = 0;

				}

				else /*Falling*/
				{	
					if(!keys[SDL_SCANCODE_SPACE])
						jumpCheck = 0;

					if(jump > 0)
					{
						jump -= moveSpeed * deltaTime;

						playerPos.y += moveSpeed * deltaTime;				
						frameTime += deltaTime;
					}
					else
						jumpCheck = 1;
				}

				/*Collision Detection*/

				if(keys[SDL_SCANCODE_T])/*Entering the Twilight Realm*/
				{
					if(transCheck == 0)
					{
						transCheck = 1;
					}
					else
					{

					}
				}

				if(keys[SDL_SCANCODE_Y])/*Entering the Real World*/
				{
					if(transCheck == 1)
					{
						transCheck = 0;
					}
					else
					{

					}
				}
				
				Camera.x = playerPos.x - 540;

				camera_live_set(Camera);

				SDL_RenderClear(renderer);
				

				if(transCheck == 0)/*Real World Platforms*/
				{
					levelDraw(backg[0], renderer, Camera);
					entity_draw(entitylist[3], 0, renderer, 640, 60, Camera);
					entity_draw(entitylist[4], 0, renderer, 1080, 0, Camera);
				}
				else/*Twilight Realms Platforms*/
				{
					levelDraw(backg[1], renderer, Camera);
					entity_draw(entitylist[2], 0, renderer, 0, 540, Camera);
				}

				entity_draw(entitylist[0], frame, renderer, playerPos.x, playerPos.y, Camera);/*Call the draw to draw the sprite to the screen*/

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
















