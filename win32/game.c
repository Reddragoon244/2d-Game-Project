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
int leverorder[5] = {0, 0, 0, 0, 0};

int moveright = 1, moveleft = 0;
int moveright2 = 1, moveleft2 = 0;
float slowTime = 0;
float slowTime2 = 0;

int i = 0;
float frameTime = 0;
float deltaTime = 0;
int jumpCheck = 1;
int jump = 0;
int jumpMax = 0;
int groundCheck = 1;
int actionCheck = 1;

void loadEntities();
void InitPos();
void puzzleLevelone(int width);
void puzzleLeveltwo(int width);
void puzzleLevelthree(int width);
void puzzleLevelfour(int width);
void bothWorlds();
int puzzleoneInfo();
void monsterInfo();
void monsterInfo2();


int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;//always make a pointer null at initial
	int playerframe = 0;//playerframe is the frame of the player sprite sheet
	int isRunning = 1;
	const Uint8 *keys;
	SDL_Event ev;
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;

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

/////////////////////////////////////*Death*/////////////////////////////////////////////
				if(entity_return_intersect_all(entitylist[0]) == entitylist[32] || 
					entity_return_intersect_all(entitylist[0]) == entitylist[7] || 
					entity_return_intersect_all(entitylist[0]) == entitylist[8])
				{
					InitPos();
				}
				else if(entitylist[0]->PositionRect.y >= 800)
				{
					InitPos();
				}

//////////////////////////////////////*Action*///////////////////////////////////////////
				if(keys[SDL_SCANCODE_RETURN] && actionCheck == 1)
				{


					if(entity_return_intersect_all(entitylist[0]) == entitylist[33])
					{
							LeverFrame[0] = 1;
							leverorder[i] = 1;
							i++;
					}

					if(entity_return_intersect_all(entitylist[0]) == entitylist[34])
					{
							LeverFrame[1] = 1;
							leverorder[i] = 2;
							i++;
					}

					if(entity_return_intersect_all(entitylist[0]) == entitylist[35])
					{
							LeverFrame[2] = 1;
							leverorder[i] = 3;
							i++;
					}

					if(entity_return_intersect_all(entitylist[0]) == entitylist[36])
					{
							LeverFrame[3] = 1;
							leverorder[i] = 4;
							i++;
					}

					if(entity_return_intersect_all(entitylist[0]) == entitylist[37])
					{
							LeverFrame[4] = 1;
							leverorder[i] = 5;
							i++;
					}
			
					actionCheck = 0;

					if(puzzleoneInfo() && i == 5)
					{
						entitylist[11]->health = 0;
						entitylist[11]->PositionRect.y = doorThink(entitylist[11], 720);
					}
					else if(i == 5)
					{
						LeverFrame[0] = 0;
						LeverFrame[1] = 0;
						LeverFrame[2] = 0;
						LeverFrame[3] = 0;
						LeverFrame[4] = 0;
						i = 0;
					}

				}

				if(!keys[SDL_SCANCODE_RETURN])
				{
					actionCheck = 1;
				}

//////////////////////////////////*Move Right*//////////////////////////////////////////
				if(keys[SDL_SCANCODE_D] && !keys[SDL_SCANCODE_A])
				{			
					moveRight(entitylist[0]->PositionRect, entitylist[0]->PositionTemp, deltaTime, entitylist[0], 1);

					    frameTime += deltaTime;
						sprite_animation(150, 143, 12.5f, frameTime, playerframe);

				}
//////////////////////////////////*Move Left*////////////////////////////////////////////				
				if(keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D])
				{	
					moveLeft(entitylist[0]->PositionRect, entitylist[0]->PositionTemp, deltaTime, entitylist[0], 1);

						frameTime += deltaTime;
						sprite_animation(123, 117, 12.5f, frameTime, playerframe);
					
				}
				
//////////////////////////////////*Jumping*//////////////////////////////////////////////

				if(keys[SDL_SCANCODE_SPACE] && jumpCheck == 1)
				{
					Jump(entitylist[0]->PositionRect, entitylist[0]->PositionTemp, deltaTime, entitylist[0], 1, jumpMax);
						frameTime += deltaTime;

						if(jumpMax == entitylist[0]->PositionRect.y)
							jumpCheck = 0;
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

						if(entity_ground_intersect_all(entitylist[0]))
						{
							jumpMax = entitylist[0]->PositionRect.y - 300;
							jumpCheck = 1;
						}
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

				Camera.x = entitylist[0]->PositionRect.x - 540;

				camera_live_set(Camera);

				SDL_RenderClear(renderer);

				monsterInfo();
				monsterInfo2();

				/*LEVEL*/
				puzzleLevelone(1080);/*Puzzle One*/
				puzzleLeveltwo(0);/*Puzzle Two*/

				/*Floor*/
				entity_draw(entitylist[40], 0, renderer, 2160, 710, Camera);

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
		spritelist[19] = sprite_load("Hbrick270.bmp", 1, 1, renderer);/*Platform Sprite Load*/
		spritelist[20] = sprite_load("Vbrick270.bmp", 1, 1, renderer);/*Platform Sprite Load*/


		entitylist[0] = entity_load(spritelist[0], 1, 1);/*Player*/
		entitylist[1] = entity_load(spritelist[1], 1, 1);/*Hplatform1080*/
		entitylist[2] = entity_load(spritelist[2], 1, 1);/*Hplatform540 puzzletwo*/
		entitylist[3] = entity_load(spritelist[3], 1, 1);/*Hplatform270*/
		entitylist[4] = entity_load(spritelist[4], 1, 1);/*Vplatform1080 puzzletwo*/
		entitylist[5] = entity_load(spritelist[5], 1, 1);/*Vplatform540*/
		entitylist[6] = entity_load(spritelist[6], 1, 1);/*Vplatform270*/
		entitylist[7] = entity_load(spritelist[7], 1, 1);/*enemy1 puzzletwo*/
		entitylist[8] = entity_load(spritelist[8], 1, 1);/*enemy2 puzzletwo*/
		entitylist[9] = entity_load(spritelist[9], 1, 1);/*Lever*/
		entitylist[10] = entity_load(spritelist[10], 1, 1);/*Box*/
		entitylist[11] = entity_load(spritelist[11], 1, 1);/*Door puzzleone*/
		entitylist[12] = entity_load(spritelist[11], 1, 1);/*Door puzzletwo*/
		entitylist[13] = entity_load(spritelist[11], 1, 1);/*Door puzzletwo*/
		entitylist[14] = entity_load(spritelist[11], 1, 1);/*Door puzzletwo*/
		entitylist[15] = entity_load(spritelist[11], 1, 1);/*Door puzzletwo*/
		entitylist[16] = entity_load(spritelist[11], 1, 1);/*Door*/
		entitylist[17] = entity_load(spritelist[9], 1, 1);/*Lever1 puzzletwo*/
		entitylist[18] = entity_load(spritelist[9], 1, 1);/*Lever2 puzzletwo*/
		entitylist[19] = entity_load(spritelist[9], 1, 1);/*Lever3 puzzletwo*/
		entitylist[20] = entity_load(spritelist[9], 1, 1);/*Lever4 puzzletwo*/
		entitylist[21] = entity_load(spritelist[9], 1, 1);/*Lever5 puzzletwo*/
		entitylist[22] = entity_load(spritelist[9], 1, 1);/*Lever6 puzzletwo*/
		entitylist[23] = entity_load(spritelist[10], 1, 1);/*Box*/
		entitylist[24] = entity_load(spritelist[10], 1, 1);/*Box*/
		entitylist[25] = entity_load(spritelist[10], 1, 1);/*Box*/
		entitylist[26] = entity_load(spritelist[12], 1, 1);/*Platform Brick One puzzleone*/
		entitylist[27] = entity_load(spritelist[13], 1, 1);/*Platform Brick Two puzzleone*/
		entitylist[28] = entity_load(spritelist[14], 1, 1);/*Platform Brick Three puzzleone*/
		entitylist[29] = entity_load(spritelist[15], 1, 1);/*Platform Brick Four puzzleone*/
		entitylist[30] = entity_load(spritelist[16], 1, 1);/*Platform Brick Five puzzleone*/
		entitylist[31] = entity_load(spritelist[17], 1, 1);/*Hplatform450 for Door puzzleone*/
		entitylist[32] = entity_load(spritelist[18], 1, 1);/*enemy3 puzzleone*/
		entitylist[33] = entity_load(spritelist[9], 1, 1);/*Lever1 puzzleone*/
		entitylist[34] = entity_load(spritelist[9], 1, 1);/*Lever2 puzzleone*/
		entitylist[35] = entity_load(spritelist[9], 1, 1);/*Lever3 puzzleone*/
		entitylist[36] = entity_load(spritelist[9], 1, 1);/*Lever4 puzzleone*/
		entitylist[37] = entity_load(spritelist[9], 1, 1);/*Lever5 puzzleone*/
		entitylist[38] = entity_load(spritelist[1], 1, 1);/*Platform Floor puzzleone*/
		entitylist[39] = entity_load(spritelist[1], 1, 1);/*Platform Floor puzzletwo*/
		entitylist[40] = entity_load(spritelist[1], 1, 1);/*Platform Floor*/
		entitylist[41] = entity_load(spritelist[17], 1, 1);/*Hplatform450 for Door puzzletwo*/
		entitylist[42] = entity_load(spritelist[4], 1, 1);/*Vplatform1080*/
		entitylist[43] = entity_load(spritelist[17], 1, 1);/*Hplatform450 for first puzzleone*/
		entitylist[44] = entity_load(spritelist[20], 1, 1);/*Vbrick270 puzzletwo 1*/
		entitylist[45] = entity_load(spritelist[20], 1, 1);/*Vbrick270 puzzletwo 2*/
		entitylist[46] = entity_load(spritelist[20], 1, 1);/*Vbrick270 puzzletwo 3*/
		entitylist[47] = entity_load(spritelist[20], 1, 1);/*Vbrick270 puzzletwo 4*/
		entitylist[48] = entity_load(spritelist[20], 1, 1);/*Vbrick270 puzzletwo 5*/
		entitylist[49] = entity_load(spritelist[20], 1, 1);/*Vbrick270 puzzletwo 6*/

}

void InitPos()
{
	transCheck = 0;
	i = 0;
	frameTime = 0;
	deltaTime = 0;
	jumpCheck = 1;
	jump = 0;
	jumpMax = 0;
	groundCheck = 1;
	actionCheck = 1;
	/*Player Init*/
	entitylist[0]->PositionRect.x = 40;
	entitylist[0]->PositionRect.y = 506;
	entitylist[0]->PositionTemp.x = 40;
	entitylist[0]->PositionTemp.y = 506;
	/*Box Init*/
	entitylist[23]->PositionRect.x = 96;
	entitylist[23]->PositionRect.y = 76;
	/*Door Init*/
	entitylist[11]->PositionRect.y = 450;
	entitylist[12]->PositionRect.y = 482;
	entitylist[13]->PositionRect.y = 482;
	entitylist[14]->PositionRect.y = 482;
	entitylist[15]->PositionRect.y = 482;
	/*Monster Puzzle One Init*/
	entitylist[32]->PositionRect.x = 638;
	entitylist[32]->PositionRect.y = 253;
	entitylist[32]->EventBounds.x = -120;
	entitylist[32]->EventBounds.y = -120;
	entitylist[32]->EventBounds.w = 240;
	entitylist[32]->EventBounds.h = 240;
	entitylist[32]->frame = 0;
	/*Monster 2 and 3*/
	entitylist[7]->PositionRect.x = 405;
	entitylist[7]->PositionRect.y = 84;
	entitylist[8]->PositionRect.x = 714;
	entitylist[8]->PositionRect.y = 292;
	entitylist[7]->frame = 0;
	entitylist[8]->frame = 0;
	/*Lever Puzzle One Init*/
	LeverFrame[0] = 0;
	LeverFrame[1] = 0;
	LeverFrame[2] = 0;
	LeverFrame[3] = 0;
	LeverFrame[4] = 0;
	/*Lever Don't Collide Init*/
	entitylist[33]->dontColl = 1;
	entitylist[34]->dontColl = 1;
	entitylist[35]->dontColl = 1;
	entitylist[36]->dontColl = 1;
	entitylist[37]->dontColl = 1;

}

void puzzleLevelone(int width)
{
	if(transCheck == 0)/*Real World Platforms*/
	{
		levelDraw(backg[0], renderer, Camera);/*Real World Background*/
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
		entitylist[36]->drawn = 0;
		entitylist[37]->drawn = 0;

	}
	else/*Twilight Realms Platforms*/
	{
		levelDraw(backg[1], renderer, Camera);/*Twilight World Background*/
		entity_draw(entitylist[30], 0, renderer, 32+width, 524, Camera);
		entity_draw(entitylist[29], 0, renderer, 370+width, 316, Camera);
		entity_draw(entitylist[37], LeverFrame[4], renderer, 76+width, 494, Camera);
		entity_draw(entitylist[36], LeverFrame[3], renderer, 500+width, 286, Camera);
		entity_draw(entitylist[32], entitylist[32]->frame, renderer, entitylist[32]->PositionRect.x+width, entitylist[32]->PositionRect.y, Camera);/*Monster*/

		/*Not Drawn*/
		entitylist[28]->drawn = 0;
		entitylist[26]->drawn = 0;
		entitylist[27]->drawn = 0;
		entitylist[33]->drawn = 0;
		entitylist[34]->drawn = 0;
		entitylist[35]->drawn = 0;

	}


	/*Both Worlds*/
	entity_draw(entitylist[38], 0, renderer, 0+width, 710, Camera);/*Floor*/
	entity_draw(entitylist[41], 0, renderer, 1048+width, 0, Camera);/*Right Side Platform 450*/
	entity_draw(entitylist[43], 0, renderer, 0+width, 0, Camera);/*Left Side*/
	entity_draw(entitylist[11], 0, renderer, 1048+width, entitylist[11]->PositionRect.y, Camera);/*Door*/
}

void puzzleLeveltwo(int width)
{
		if(transCheck == 0)/*Real World Platforms*/
	{
		entity_draw(entitylist[47], 0, renderer, 96+width, 116, Camera);
		entity_draw(entitylist[48], 0, renderer, 405+width, 324, Camera);
		entity_draw(entitylist[49], 0, renderer, 714+width, 116, Camera);

		/*Not Drawn*/
		entitylist[44]->drawn = 0;
		entitylist[45]->drawn = 0;
		entitylist[46]->drawn = 0;
		entitylist[7]->drawn = 0;
		entitylist[8]->drawn = 0;

	}
	else/*Twilight Realms Platforms*/
	{
		entity_draw(entitylist[44], 0, renderer, 96+width, 324, Camera);
		entity_draw(entitylist[45], 0, renderer, 405+width, 116, Camera);
		entity_draw(entitylist[46], 0, renderer, 714+width, 324, Camera);
		entity_draw(entitylist[7], entitylist[7]->frame, renderer, entitylist[7]->PositionRect.x+width, entitylist[7]->PositionRect.y, Camera);
		entity_draw(entitylist[8], entitylist[8]->frame, renderer, entitylist[8]->PositionRect.x+width, entitylist[8]->PositionRect.y, Camera);

		/*Not Drawn*/
		entitylist[47]->drawn = 0;
		entitylist[48]->drawn = 0;
		entitylist[49]->drawn = 0;

	}

	/*Both Worlds*/
	entity_draw(entitylist[2], 0, renderer, 542+width, 450, Camera);/*Platform Door Holder*/
	entity_draw(entitylist[39], 0, renderer, 0+width, 710, Camera);/*Floor*/
	entity_draw(entitylist[31], 0, renderer, 1048+width, 0, Camera);/*Right Side*/
	entity_draw(entitylist[4], 0, renderer, 0+width, 0, Camera);/*Left Side*/
	entity_draw(entitylist[12], 0, renderer, 542+width, entitylist[12]->PositionRect.y, Camera);/*Door One*/
	entity_draw(entitylist[13], 0, renderer, 692+width, entitylist[13]->PositionRect.y, Camera);/*Door Two*/
	entity_draw(entitylist[14], 0, renderer, 842+width, entitylist[14]->PositionRect.y, Camera);/*Door Three*/
	entity_draw(entitylist[15], 0, renderer, 992+width, entitylist[15]->PositionRect.y, Camera);/*Door Four*/
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

int puzzleoneInfo ()
{
	int leverOrder[5] = {1, 2, 3, 4, 5};

	if(leverOrder[0] == leverorder[0] && leverOrder[1] == leverorder[1] && leverOrder[2] == leverorder[2] && leverOrder[3] == leverorder[3] && leverOrder[4] == leverorder[4])
	{
		return 1;
	}
	else
		return 0;
}

void monsterInfo()
{

	/*Monster*/

			if(moveright == 1)
			{
				slowTime++;

				if(slowTime == 150)
				{
					entitylist[7]->PositionRect.x += 5;
					slowTime = 0;
				}

				if(entitylist[7]->PositionRect.x >= 650)
				{
					moveright = 0;
					moveleft = 1;
				}

			}
			else if(moveleft == 1)
			{
				slowTime++;

				if(slowTime == 150)
				{
					entitylist[7]->PositionRect.x -= 5;
					slowTime = 0;
				}

				if(entitylist[7]->PositionRect.x <= 405)
				{
					moveleft = 0;
					moveright = 1;
				}
			}

			if(entity_intersect_event_all(entitylist[0]))
			{
				/*Monster Animation*/
 				sprite_animation(8, 0, 32.5f, frameTime, entitylist[32]->frame);
			}
			else
				entitylist[32]->frame = 0;
}

void monsterInfo2()
{

	/*Monster*/

			if(moveright2 == 1)
			{
				slowTime2++;

				if(slowTime2 == 150)
				{
					entitylist[8]->PositionRect.x += 5;
					slowTime2 = 0;
				}

				if(entitylist[8]->PositionRect.x >= 914)
				{
					moveright2 = 0;
					moveleft2 = 1;
				}

			}
			else if(moveleft2 == 1)
			{
				slowTime2++;

				if(slowTime2 == 150)
				{
					entitylist[8]->PositionRect.x -= 5;
					slowTime2 = 0;
				}

				if(entitylist[8]->PositionRect.x <= 714)
				{
					moveleft2 = 0;
					moveright2 = 1;
				}
			}

}




