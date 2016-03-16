#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "sprite.h"
#include "entity.h"
#include "vector.h"
#include "level.h"
#include "Camera.h"
#include "Animation.h"
#include "items.h"
#include "physics.h"
#include <string>

SDL_Renderer *renderer;
SDL_Texture *texture;
Sprite *spritelist[spriteMax];
Entity_t *entitylist[entityMax];
Level *backg[levelMax];

int transCheck = 0;
int LeverFrame[entityMax];

void loadEntities();
void InitPos();
void puzzleLevelone(int width);
void puzzleLeveltwo(int width);
void puzzleLevelthree(int width);
void puzzleLevelfour(int width);
void bothWorlds();
int puzzleoneInfo(int order[]);


int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;//always make a pointer null at initial
	int leverorder[5] = {0, 0, 0, 0, 0};
	int playerframe = 0;//playerframe is the frame of the player sprite sheet
	int isRunning = 1;
	const Uint8 *keys;
	SDL_Event ev;
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	float frameTime = 0;
	float deltaTime = 0, slowTime = 0, slowTimeInit = 500;
	int jumpCheck = 1;
	int jump = 0;
	int jumpMax = 200;
	int groundCheck = 1;
	int leverFrame = 0;
	int boxGrab = 0;

		window = SDL_CreateWindow("Into the Twilight", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 720, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);	
		
		if(!(IMG_Init(imgFlags) & imgFlags))
			fprintf(stderr, "Error INIT PNG or JPG", SDL_GetError());

		loadEntities();
		InitPos();

		playerframe = 143;/*starting frame*/
		backg[2] = backg[0];

			while(isRunning)
			{
				deltaTime = time_animation();

				while(SDL_PollEvent(&ev) != 0)
				{
					if(ev.type == SDL_QUIT)
						isRunning = 0;
				}

				keys = SDL_GetKeyboardState(NULL);

//////////////////////////////////////*Action*///////////////////////////////////////////
				if(keys[SDL_SCANCODE_RETURN])
				{

					if(entity_return_intersect_all(entitylist[0]) == entitylist[33])
					{
							LeverFrame[0] = 1;
							leverorder[0] = 1;
					}

					if(entity_return_intersect_all(entitylist[0]) == entitylist[34])
					{
							LeverFrame[1] = 1;
							leverorder[1] = 2;
					}

					if(entity_return_intersect_all(entitylist[0]) == entitylist[35])
					{
							LeverFrame[2] = 1;
							leverorder[2] = 3;
					}

				}

//////////////////////////////////*Move Right*//////////////////////////////////////////
				if(keys[SDL_SCANCODE_D] && !keys[SDL_SCANCODE_A])
				{			
					moveRight(entitylist[0]->PositionRect, entitylist[0]->PositionTemp, deltaTime, entitylist[0], 1);

					    frameTime += deltaTime;
						playerframe = sprite_animation(150, 143, 12.5f, frameTime);

				}
//////////////////////////////////*Move Left*////////////////////////////////////////////				
				if(keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D])
				{	
					moveLeft(entitylist[0]->PositionRect, entitylist[0]->PositionTemp, deltaTime, entitylist[0], 1);

						frameTime += deltaTime;
						playerframe = sprite_animation(123, 117, 12.5f, frameTime);
					
				}
				
//////////////////////////////////*Jumping*//////////////////////////////////////////////

				if(keys[SDL_SCANCODE_SPACE] && jumpCheck == 1)
				{

					Jump(entitylist[0]->PositionRect, entitylist[0]->PositionTemp, deltaTime, entitylist[0], 1);
							frameTime += deltaTime;
						

				}
//////////////////////////////////*Falling*//////////////////////////////////////////////
				else 
				{	
					if(!keys[SDL_SCANCODE_SPACE])
						jumpCheck = 0;

					if(jumpCheck == 0)
					{
						Fall(entitylist[0]->PositionRect, entitylist[0]->PositionTemp, deltaTime, entitylist[0], 1);
							frameTime += deltaTime;

						if(keys[SDL_SCANCODE_SPACE])
							jumpCheck = 1;
					}
					
				}

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

				if(puzzleoneInfo(leverorder))
				{
					entitylist[11]->health = 0;
					entitylist[11]->PositionRect.y = doorThink(entitylist[11], 720);
				}

				Camera.x = entitylist[0]->PositionRect.x - 540;

				camera_live_set(Camera);

				SDL_RenderClear(renderer);

				if(entity_intersect_event_all(entitylist[0]))
				{
					/*Monster Animation*/
					if(slowTime == slowTimeInit)
					{
						entitylist[32]->frame++;
						slowTime = 0;

						if(entitylist[32]->frame == 8)
							entitylist[32]->frame = 1;
					}
					else
						slowTime++;
				}
				/*LEVEL*/
				puzzleLevelone(0);/*Puzzle One*/

				/*Player*/entity_draw(entitylist[0], playerframe, renderer, entitylist[0]->PositionRect.x, entitylist[0]->PositionRect.y, Camera);/*Call the draw to draw the sprite to the screen*/

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

void loadEntities()
{
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
		spritelist[11] = sprite_load("door.bmp", 1, 1, renderer);/*Function to load the sprite file into the array Sprite List*/
		spritelist[12] = sprite_load("onebrick.bmp", 1, 1, renderer);/*Function to load the sprite file into the array Sprite List*/
		spritelist[13] = sprite_load("twobrick.bmp", 1, 1, renderer);/*Function to load the sprite file into the array Sprite List*/
		spritelist[14] = sprite_load("threebrick.bmp", 1, 1, renderer);/*Function to load the sprite file into the array Sprite List*/
		spritelist[15] = sprite_load("fourbrick.bmp", 1, 1, renderer);/*Function to load the sprite file into the array Sprite List*/
		spritelist[16] = sprite_load("fivebrick.bmp", 1, 1, renderer);/*Function to load the sprite file into the array Sprite List*/
		spritelist[17] = sprite_load("Hplatform450.bmp",1, 1, renderer);/*Platform Sprite Load*/
		spritelist[18] = sprite_load("enemy3.bmp", 8, 1, renderer);/*Function to load the sprite file into the array Sprite List*/


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
		entitylist[11] = entity_load(spritelist[11], 1, 1);/*Function to load the sprite into the entitylist */
		entitylist[12] = entity_load(spritelist[11], 1, 1);/*Door*/
		entitylist[13] = entity_load(spritelist[11], 1, 1);/*Door*/
		entitylist[14] = entity_load(spritelist[11], 1, 1);/*Door*/
		entitylist[15] = entity_load(spritelist[11], 1, 1);/*Door*/
		entitylist[16] = entity_load(spritelist[11], 1, 1);/*Door*/
		entitylist[17] = entity_load(spritelist[9], 1, 1);/*Lever*/
		entitylist[18] = entity_load(spritelist[9], 1, 1);/*Lever*/
		entitylist[19] = entity_load(spritelist[9], 1, 1);/*Lever*/
		entitylist[20] = entity_load(spritelist[9], 1, 1);/*Lever*/
		entitylist[21] = entity_load(spritelist[9], 1, 1);/*Lever*/
		entitylist[22] = entity_load(spritelist[9], 1, 1);/*Lever*/
		entitylist[23] = entity_load(spritelist[10], 1, 1);/*Box*/
		entitylist[24] = entity_load(spritelist[10], 1, 1);/*Box*/
		entitylist[25] = entity_load(spritelist[10], 1, 1);/*Box*/
		entitylist[26] = entity_load(spritelist[12], 1, 1);/*Function to load the sprite into the entitylist */
		entitylist[27] = entity_load(spritelist[13], 1, 1);/*Function to load the sprite into the entitylist */
		entitylist[28] = entity_load(spritelist[14], 1, 1);/*Function to load the sprite into the entitylist */
		entitylist[29] = entity_load(spritelist[15], 1, 1);/*Function to load the sprite into the entitylist */
		entitylist[30] = entity_load(spritelist[16], 1, 1);/*Function to load the sprite into the entitylist */
		entitylist[31] = entity_load(spritelist[17], 1, 1);/*Function to load the sprite into the entitylist */
		entitylist[32] = entity_load(spritelist[18], 1, 1);/*Function to load the sprite into the entitylist */
		entitylist[33] = entity_load(spritelist[9], 1, 1);/*Lever1*/
		entitylist[34] = entity_load(spritelist[9], 1, 1);/*Lever2*/
		entitylist[35] = entity_load(spritelist[9], 1, 1);/*Lever3*/
		entitylist[36] = entity_load(spritelist[9], 1, 1);/*Lever4*/
		entitylist[37] = entity_load(spritelist[9], 1, 1);/*Lever5*/

}

void InitPos()
{
	entitylist[0]->PositionRect.x = 0;
	entitylist[0]->PositionRect.y = 656;
	entitylist[0]->PositionTemp.x = 0;
	entitylist[0]->PositionTemp.y = 656;

	entitylist[23]->PositionRect.x = 96;
	entitylist[23]->PositionRect.y = 76;

	entitylist[11]->PositionRect.x = 1080;
	entitylist[11]->PositionRect.y = 450;

	entitylist[32]->PositionRect.x = 638;
	entitylist[32]->PositionRect.y = 253;
	entitylist[32]->EventBounds.x = -120;
	entitylist[32]->EventBounds.y = -120;
	entitylist[32]->EventBounds.w = 240;
	entitylist[32]->EventBounds.h = 240;
	entitylist[32]->frame = 0;

	LeverFrame[0] = 0;
	LeverFrame[1] = 0;
	LeverFrame[2] = 0;

}

void puzzleLevelone(int width)
{
	if(transCheck == 0)/*Real World Platforms*/
	{
		levelDraw(backg[0], renderer, Camera);
		entity_draw(entitylist[28], 0, renderer, 32+width, 316, Camera);
		entity_draw(entitylist[26], 0, renderer, 400+width, 108, Camera);
		entity_draw(entitylist[27], 0, renderer, 708+width, 316, Camera);
		entity_draw(entitylist[33], LeverFrame[0], renderer, 430+width, 78, Camera);
		entity_draw(entitylist[34], LeverFrame[1], renderer, 738+width, 286, Camera);
		entity_draw(entitylist[35], LeverFrame[2], renderer, 76+width, 286, Camera);

		/*Not Drawn*/
		entitylist[30]->drawn = 0;
		entitylist[29]->drawn = 0;
		entitylist[32]->drawn = 0;
	}
	else/*Twilight Realms Platforms*/
	{
		levelDraw(backg[1], renderer, Camera);
		entity_draw(entitylist[30], 0, renderer, 32+width, 524, Camera);
		entity_draw(entitylist[29], 0, renderer, 370+width, 316, Camera);
		entity_draw(entitylist[32], entitylist[32]->frame, renderer, entitylist[32]->PositionRect.x, entitylist[32]->PositionRect.y, Camera);/*Monster*/

		/*Not Drawn*/
		entitylist[28]->drawn = 0;
		entitylist[26]->drawn = 0;
		entitylist[27]->drawn = 0;

	}


	/*Both Worlds*/
	entity_draw(entitylist[31], 0, renderer, 1048+width, 0, Camera);
	entity_draw(entitylist[11], 0, renderer, 1048+width, entitylist[11]->PositionRect.y, Camera);
}

void puzzleLeveltwo(int width)
{

}

void puzzleLevelthree(int width)
{

}

void puzzleLevelfour(int width)
{

}

void bothWorlds()
{

}

int puzzleoneInfo (int order[])
{
	int leverOrder[5] = {1, 2, 3, 4, 5};

	if(leverOrder == order)
	{
		return 1;
	}
	else
		return 0;
}







