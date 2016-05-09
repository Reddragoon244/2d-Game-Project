#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include "font.h"
#include "sound.h"
#include "sprite.h"
#include "entity.h"
#include "vector.h"
#include "level.h"
#include "Camera.h"
#include "Animation.h"
#include "items.h"
#include "physics.h"
#include "widget.h"
#include "fileIO.h"
#include "editor.h"

SDL_Renderer *renderer;
SDL_Texture *texture;
Sprite *spritelist[spriteMax];
Entity_t *entitylist[entityMax];
Level *backg[levelMax];
Music *musicList[MAX_MUSIC];
SoundEffect *soundList[MAX_SOUND];
Font *fontList[MAX_FONT];
Widget *widgetList[MAX_WIDGET];

SDL_Rect mouse = {0, 0, 0, 0};
SDL_Rect editorMouse = {0, 0, 0, 0};

int transCheck = 0;
int LeverFrame[entityMax];
int leverorder[5] = {0, 0, 0, 0, 0};

int moveright = 1, moveleft = 0;
int moveright2 = 1, moveleft2 = 0;
float slowTime = 0;
float slowTime2 = 0;
float slowTime3 = 0;
float gameOverTime = 0;
int level = 33;
int scene = 0;
int pausegame = 0;

int spriteLoad = 0;
int contentEnder = 0;
float meterPower = 1;
float meterNumber = 1.5;
int helpTrue = 1;
int editorSprite = 99;
int i = 0;
float frameTime = 0;
float deltaTime = 0;
int jumpCheck = 1;
int jump = 0;
int jumpMax = 0;
int groundCheck = 1;
int actionCheck = 1;
int gameOver = 0;

void Meter();
void editorSave();
void editorLoad();
void editor();
void createEntity(int i, int x, int y);
void load();
void save();
void loadEntities();
void InitPos();
void puzzleLevelone(int width);
void puzzleLeveltwo(int width);
void puzzleLevelthree(int width);
void puzzleLevelfour(int width);
void ChangeWorlds();
void LevelOrder();
int puzzleoneInfo();
void monsterInfo(Entity_t *a);
void monsterInfo2(int leftMax, int rightMax, int width);
void monsterInfo4(int leftMax, int rightMax, int width, int teleDistance);
void monsterInfo3(int width);
void leverAction();
void boss();


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

		InitSoundSystem();
		InitFontSystem();
		loadEntities();
		InitPos();

		playerframe = 143;/*starting frame*/
		backg[5] = backg[0];



		if(Mix_PlayMusic(musicList[0]->music, -1) == -1)
			printf("Mix_PlayMusic: %s\n", Mix_GetError());

/////////////////////////////////////*Game*//////////////////////////////////////////////
			while(isRunning)
			{
				deltaTime = time_animation();

				while(SDL_PollEvent(&ev) != 0)
				{
					if(ev.type == SDL_QUIT)
					{
						isRunning = 0;
					}else if(ev.type == SDL_MOUSEBUTTONDOWN)//for editor
					{
						mouse.x = ev.button.x;
						mouse.y = ev.button.y;

						if(scene == 2)
						{
							if(editorSprite != 99)
								createEntity(editorSprite, editorMouse.x, editorMouse.y); 
						}
					}
					else//store the position of the event
					{
						if(scene == 2 && pausegame == 0)
						{
							editorMouse.x = ev.button.x + Camera.x;
							editorMouse.y = ev.button.y;
							printf("%i, %i \n", editorMouse.x, editorMouse.y);
						}
					}

				}

				keys = SDL_GetKeyboardState(NULL);

///////////////////////////////////*Main Menu*///////////////////////////////////////////
			if(scene == 0)
			{
				font_draw(fontList[0], renderer, 140, 40, Camera, 1);

				widgetList[0]->draw(widgetList[0], 0, renderer, 210, 240, Camera, 4, 4);
				widgetList[3]->draw(widgetList[3], 0, renderer, 210, 440, Camera, 4, 4);
				widgetList[2]->draw(widgetList[2], 0, renderer, 610, 240, Camera, 4, 4);
				widgetList[4]->draw(widgetList[4], 0, renderer, 610, 440, Camera, 4, 4);

				if(widgetList[0]->update(widgetList[0], mouse))
				{
					scene = 1;
					level = 1;
					mouse.x = 0;
					mouse.y = 0;
				}

				if(widgetList[2]->update(widgetList[2], mouse))
				{
					scene = 1;
					level = 1;
					mouse.x = 0;
					mouse.y = 0;
					load();
				}

				if(widgetList[3]->update(widgetList[3], mouse))/*Editor*/
				{
					scene = 2;
				}

				if(widgetList[4]->update(widgetList[4], mouse))
				{
					mouse.x = 0;
					mouse.y = 0;
					isRunning = false;
				}

			}

/////////////////////////////////////*GAME*//////////////////////////////////////////////
			if(scene == 1 || scene == 2)
			{
/////////////////////////////////////*Death*/////////////////////////////////////////////
				if(entity_return_intersect_all(entitylist[0]) == entitylist[32] || 
					entity_return_intersect_all(entitylist[0]) == entitylist[7] || 
					entity_return_intersect_all(entitylist[0]) == entitylist[8] ||
					entity_return_intersect_all(entitylist[0]) == entitylist[50] ||
					entity_return_intersect_all(entitylist[0]) == entitylist[51] ||
					entity_return_intersect_all(entitylist[0]) == entitylist[58])
				{
					InitPos();
					printf("death 1");
				}
				else if(entitylist[0]->PositionRect.y >= 800)
				{
					InitPos();
					printf("death 2");
				}
			if(pausegame == 0)
			{
//////////////////////////////////////*Action*///////////////////////////////////////////
				if(keys[SDL_SCANCODE_RETURN] && actionCheck == 1 && gameOver == 0)
				{
					leverAction();	
				}

				if(keys[SDL_SCANCODE_RETURN] && actionCheck == 1 && gameOver == 1)
				{
					InitPos();
					level = 1;
				}

				if(!keys[SDL_SCANCODE_RETURN])
				{
					actionCheck = 1;
				}

//////////////////////////////////*Move Right*//////////////////////////////////////////
				if(keys[SDL_SCANCODE_D] && !keys[SDL_SCANCODE_A] && gameOver == 0)
				{			
					moveRight(entitylist[0]->PositionRect, entitylist[0]->PositionTemp, deltaTime, entitylist[0], 1);

					    frameTime += deltaTime;
						sprite_animation(150, 143, 12.5f, frameTime, playerframe);
				}
//////////////////////////////////*Move Left*////////////////////////////////////////////				
				if(keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D] && gameOver == 0)
				{	
					moveLeft(entitylist[0]->PositionRect, entitylist[0]->PositionTemp, deltaTime, entitylist[0], 1);

						frameTime += deltaTime;
						sprite_animation(123, 117, 12.5f, frameTime, playerframe);
				}

//////////////////////////////////*Jumping*//////////////////////////////////////////////

				if(keys[SDL_SCANCODE_SPACE] && jumpCheck == 1 && gameOver == 0)
				{
					if(soundList[1]->repeat == 0)
					{
						Mix_PlayChannel(-1, soundList[1]->seffect, 0);
						soundList[1]->repeat = 1;
					}
					Jump(entitylist[0]->PositionRect, entitylist[0]->PositionTemp, deltaTime, entitylist[0], 1, jumpMax);
						frameTime += deltaTime;

						if(jumpMax == entitylist[0]->PositionRect.y)
							jumpCheck = 0;
				}

//////////////////////////////////*Falling*//////////////////////////////////////////////
				else if(gameOver == 0)
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
							soundList[1]->repeat = 0;
						}
					}
					
				}
///////////////////////////////////*Changing Realms*////////////////////////////////////////////
				if(keys[SDL_SCANCODE_T] && gameOver == 0 && meterPower == 1)/*Entering the Twilight Realm*/
				{
					if(transCheck == 0)
					{
						transCheck = 1;
					}
					else
					{
						transCheck = 1;
					}

				}

				if(keys[SDL_SCANCODE_Y] && gameOver == 0)/*Entering the Real World*/
				{
					if(transCheck == 1)
					{
						transCheck = 0;
					}
					else
					{
						transCheck = 0;
					}

				}

				/*Door Thinks*/
				entity_update_all();

				if(Camera.x > 2161)
				{
					if(entitylist[0]->PositionRect.x-540 == 2160)
						Camera.x = entitylist[0]->PositionRect.x - 540;
				}
				else
					Camera.x = entitylist[0]->PositionRect.x - 540;

				camera_live_set(Camera);
				
				SDL_RenderClear(renderer);

				ChangeWorlds();
				
				if(scene == 2)
				{
					if(editorControls() == 0)
					{

					}
					else
						editorSprite = editorControls();

					editor();
				}
				else
				{
					Meter();
					font_draw(fontList[12], renderer, 400, 20, Camera, 1);
					widgetList[6]->draw(widgetList[6], 0, renderer, 5, 5, Camera, 1.5, 1.5);
					widgetList[7]->draw(widgetList[7], 0, renderer, 4, 5, Camera, meterNumber, 1.5);/*UI METER*/
				}
				
				if(scene == 1)
					LevelOrder();

				/*Player*/entity_draw(entitylist[0], playerframe, renderer, entitylist[0]->PositionRect.x, entitylist[0]->PositionRect.y, Camera);/*Call the draw to draw the sprite to the screen*/

			}
////////////////////////////////////*PAUSE MENU*/////////////////////////////////////////

				if(keys[SDL_SCANCODE_ESCAPE] || pausegame == 1)
				{
					pausegame = 1;

					widgetList[5]->draw(widgetList[5], 0, renderer, 410, 150, Camera, 3, 3);
					widgetList[2]->draw(widgetList[2], 0, renderer, 410, 300, Camera, 3, 3);
					widgetList[1]->draw(widgetList[1], 0, renderer, 410, 450, Camera, 3, 3);
					widgetList[4]->draw(widgetList[4], 0, renderer, 410, 600, Camera, 3, 3);

					if(widgetList[5]->update(widgetList[5], mouse))
					{
						pausegame = 0;
						mouse.x = 0;
						mouse.y = 0;
					}
	
					if(widgetList[2]->update(widgetList[2], mouse))
					{
						if(scene == 2)
						{
							editorLoad();
						}
						else
							load();

						mouse.x = 0;
						mouse.y = 0;
					}

					if(widgetList[1]->update(widgetList[1], mouse))
					{
						if(scene == 2)
						{
							editorSave();
						}
						else
							save();

						mouse.x = 0;
						mouse.y = 0;
					}

					if(widgetList[4]->update(widgetList[4], mouse))
					{
						mouse.x = 0;
						mouse.y = 0;
						isRunning = false;
					}
					
				}

				if(keys[SDL_SCANCODE_H] && scene == 2 && helpTrue == 1)
				{
					font_draw(fontList[1], renderer, 0, 40, Camera, 1);
					font_draw(fontList[2], renderer, 0, 80, Camera, 1);
					font_draw(fontList[3], renderer, 0, 120, Camera, 1);
					font_draw(fontList[4], renderer, 0, 160, Camera, 1);
					font_draw(fontList[5], renderer, 0, 200, Camera, 1);
					font_draw(fontList[6], renderer, 0, 240, Camera, 1);
					font_draw(fontList[7], renderer, 0, 280, Camera, 1);
					font_draw(fontList[8], renderer, 0, 320, Camera, 1);
					font_draw(fontList[9], renderer, 0, 360, Camera, 1);
					font_draw(fontList[12], renderer, 0, 400, Camera, 1);
					font_draw(fontList[10], renderer, 0, 440, Camera, 1);
					font_draw(fontList[11], renderer, 0, 480, Camera, 1);
					
					if(!keys[SDL_SCANCODE_H])
						helpTrue = 0;
				}
				else
					helpTrue = 1;

			}
				SDL_RenderPresent(renderer);
			}
		
	
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	window = NULL;
	texture = NULL;
	renderer = NULL;

	IMG_Quit();
	Mix_Quit();
	SDL_Quit();

	return 0;
}

void loadEntities()
{
		initSpriteSystem();/*The initialization of the sprite system*/
		initEntitySystem();/*The initialization of the entity system*/

		backg[0] = levelloadbg("background.png", renderer);
	    backg[1] = levelloadbg("background2.png", renderer);
		backg[2] = levelloadbg("bosslevel.png", renderer);
		backg[3] = levelloadbg("bosslevel2.png", renderer);

		musicList[0] = loadMusic("sounds/DontStopMeNow.mp3");
		soundList[0] = loadSound("sounds/switch.wav");
		soundList[1] = loadSound("sounds/jumping.wav");

		spritelist[0] = sprite_load("image.bmp", 13, 21, renderer, 0);/*Function to load the sprite file into the array Sprite List*/
		spritelist[1] = sprite_load("Hplatform1080.bmp", 1, 1, renderer, 1);/*Platform Sprite Load*/
		spritelist[2] = sprite_load("Hplatform540.bmp",1, 1, renderer, 2);/*Platform Sprite Load*/
		spritelist[3] = sprite_load("Hplatform270.bmp",1, 1, renderer, 3);/*Platform Sprite Load*/
		spritelist[4] = sprite_load("Vplatform1080.bmp",1, 1, renderer, 4);/*Platform Sprite Load*/
		spritelist[5] = sprite_load("Vplatform540.bmp",1, 1, renderer, 5);/*Platform Sprite Load*/
		spritelist[6] = sprite_load("Vplatform270.bmp",1, 1, renderer, 6);/*Platform Sprite Load*/
		spritelist[7] = sprite_load("enemy1.bmp", 3, 1, renderer, 7);/*Function to load the sprite file into the array Sprite List*/
		spritelist[8] = sprite_load("enemy2.bmp", 3, 1, renderer, 8);/*Function to load the sprite file into the array Sprite List*/
		spritelist[9] = sprite_load("lever.bmp", 2, 1, renderer, 9);/*Function to load the sprite file into the array Sprite List*/
		spritelist[10] = sprite_load("box.bmp", 1, 1, renderer, 10);/*Function to load the sprite file into the array Sprite List*/
		spritelist[11] = sprite_load("door.bmp", 1, 1, renderer, 11);/*Function to load the sprite file into the array Sprite List*/
		spritelist[12] = sprite_load("onebrick.bmp", 1, 1, renderer, 12);/*Function to load the sprite file into the array Sprite List*/
		spritelist[13] = sprite_load("twobrick.bmp", 1, 1, renderer, 13);/*Function to load the sprite file into the array Sprite List*/
		spritelist[14] = sprite_load("threebrick.bmp", 1, 1, renderer, 14);/*Function to load the sprite file into the array Sprite List*/
		spritelist[15] = sprite_load("fourbrick.bmp", 1, 1, renderer, 15);/*Function to load the sprite file into the array Sprite List*/
		spritelist[16] = sprite_load("fivebrick.bmp", 1, 1, renderer, 16);/*Function to load the sprite file into the array Sprite List*/
		spritelist[17] = sprite_load("Hplatform450.bmp",1, 1, renderer, 17);/*Platform Sprite Load*/
		spritelist[18] = sprite_load("enemy3.bmp", 8, 1, renderer, 18);/*Function to load the sprite file into the array Sprite List*/
		spritelist[19] = sprite_load("Hbrick270.bmp", 1, 1, renderer, 19);/*Platform Sprite Load*/
		spritelist[20] = sprite_load("Vbrick270.bmp", 1, 1, renderer, 20);/*Platform Sprite Load*/
		spritelist[21] = sprite_load("spikes.bmp", 1, 1, renderer, 21);/*Platform Sprite Load*/
		spritelist[22] = sprite_load("enemy4.bmp", 8, 1, renderer, 22);/*Platform Sprite Load*/
		spritelist[23] = sprite_load("eyes.bmp", 3, 1, renderer, 23);/*Platform Sprite Load*/
		spritelist[24] = sprite_load("youwin.bmp", 1, 1, renderer, 24);/*Platform Sprite Load*/
		spritelist[25] = sprite_load("StartButton.bmp", 1, 1, renderer, 25);/*Menu Button*/
		spritelist[26] = sprite_load("SaveButton.bmp", 1, 1, renderer, 26);/*Menu Button*/
		spritelist[27] = sprite_load("LoadButton.bmp", 1, 1, renderer, 27);/*Menu Button*/
		spritelist[28] = sprite_load("EditorButton.bmp", 1, 1, renderer, 28);/*Menu Button*/
		spritelist[29] = sprite_load("ExitButton.bmp", 1, 1, renderer, 29);/*Menu Button*/
		spritelist[30] = sprite_load("ResumeButton.bmp", 1, 1, renderer, 30);/*Menu Button*/
		spritelist[31] = sprite_load("meterborder.bmp", 1, 1, renderer, 31);/*Meter Border*/
		spritelist[32] = sprite_load("meter.bmp", 1, 1, renderer, 32);/*Meter*/

		fontList[0] = loadFont("fonts/Coalition.ttf", 60, "Into the Twilight", renderer);
		fontList[1] = loadFont("fonts/font.ttf", 40, "Welcome to the Into The Twilight Level Editor.", renderer);
		fontList[2] = loadFont("fonts/font.ttf", 40, " 1 - Vertical platform 1080 pixels.", renderer);
		fontList[3] = loadFont("fonts/font.ttf", 40, " 2 - Vertical platform 540 pixels.", renderer);
		fontList[4] = loadFont("fonts/font.ttf", 40, " 3 - Vertical platform 270 pixels.", renderer);
		fontList[5] = loadFont("fonts/font.ttf", 40, " 4 - Horizantal platform 1080 pixels.", renderer);
		fontList[6] = loadFont("fonts/font.ttf", 40, " 5 - Horizantal platform 540 pixels.", renderer);
		fontList[7] = loadFont("fonts/font.ttf", 40, " 6 - Horizantal platform 270 pixels.", renderer);
		fontList[8] = loadFont("fonts/font.ttf", 40, " 7 - Lever.", renderer);
		fontList[9] = loadFont("fonts/font.ttf", 40, " 8 - Door.", renderer);
		fontList[12] = loadFont("fonts/font.ttf", 40, " Place the Door First before Lever", renderer);
		fontList[10] = loadFont("fonts/font.ttf", 30, " Press Left Click after the Number to Place the Item.", renderer);
		fontList[11] = loadFont("fonts/font.ttf", 40, " Help", renderer);
		fontList[12] = loadFont("fonts/Coalition.ttf", 30, "Editor", renderer);

		widgetList[0] = loadWidget(spritelist[25], fontList[0], 4);/*Widget Start Button*/
		widgetList[1] = loadWidget(spritelist[26], fontList[0], 4);/*Widget Save Button*/
		widgetList[2] = loadWidget(spritelist[27], fontList[0], 4);/*Widget Load Button*/
		widgetList[3] = loadWidget(spritelist[28], fontList[0], 4);/*Widget Editor Button*/
		widgetList[4] = loadWidget(spritelist[29], fontList[0], 4);/*Widget Exit Button*/
		widgetList[5] = loadWidget(spritelist[30], fontList[0], 4);/*Widget Resume Button*/
		widgetList[6] = loadWidget(spritelist[31], fontList[0], 1);/*Widget Meter Border*/
		widgetList[7] = loadWidget(spritelist[32], fontList[0], 1);/*Widget Meter*/

		entitylist[0] = entity_load(spritelist[0], 1, 1);/*Player*/
		entitylist[1] = entity_load(spritelist[1], 1, 1);/*Hplatform1080*/
		entitylist[2] = entity_load(spritelist[2], 1, 1);/*Hplatform540 puzzletwo*/
		entitylist[3] = entity_load(spritelist[3], 1, 1);/*Hplatform270*/
		entitylist[4] = entity_load(spritelist[4], 1, 1);/*Vplatform1080 puzzletwo*/
		entitylist[5] = entity_load(spritelist[5], 1, 1);/*Vplatform540*/
		entitylist[6] = entity_load(spritelist[6], 1, 1);/*Vplatform270*/
		entitylist[7] = entity_load(spritelist[7], 1, 1);/*enemy1 puzzletwo*/
		entitylist[8] = entity_load(spritelist[8], 1, 1);/*enemy2 puzzletwo*/
		entitylist[9] = entity_load(spritelist[9], 1, 1);/*Lever puzzlethree*/
		entitylist[10] = entity_load(spritelist[10], 1, 1);/*Box*/
		entitylist[11] = entity_load(spritelist[11], 1, 1);/*Door puzzleone*/
		entitylist[12] = entity_load(spritelist[11], 1, 1);/*Door puzzletwo*/
		entitylist[13] = entity_load(spritelist[11], 1, 1);/*Door puzzletwo*/
		entitylist[14] = entity_load(spritelist[11], 1, 1);/*Door puzzletwo*/
		entitylist[15] = entity_load(spritelist[11], 1, 1);/*Door puzzletwo*/
		entitylist[16] = entity_load(spritelist[11], 1, 1);/*Door puzzlethree*/
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
		entitylist[40] = entity_load(spritelist[1], 1, 1);/*Platform Floor puzzlethree*/
		entitylist[41] = entity_load(spritelist[17], 1, 1);/*Hplatform450 for Door puzzletwo*/
		entitylist[42] = entity_load(spritelist[4], 1, 1);/*Vplatform1080*/
		entitylist[43] = entity_load(spritelist[17], 1, 1);/*Hplatform450 for first puzzleone*/
		entitylist[44] = entity_load(spritelist[20], 1, 1);/*Vbrick270 puzzletwo 1*/
		entitylist[45] = entity_load(spritelist[20], 1, 1);/*Vbrick270 puzzletwo 2*/
		entitylist[46] = entity_load(spritelist[20], 1, 1);/*Vbrick270 puzzletwo 3*/
		entitylist[47] = entity_load(spritelist[20], 1, 1);/*Vbrick270 puzzletwo 4*/
		entitylist[48] = entity_load(spritelist[20], 1, 1);/*Vbrick270 puzzletwo 5*/
		entitylist[49] = entity_load(spritelist[20], 1, 1);/*Vbrick270 puzzletwo 6*/
		entitylist[50] = entity_load(spritelist[21], 1, 1);/*SPIKES*/
		entitylist[51] = entity_load(spritelist[22], 1, 1);/*Spike Enemy*/
		entitylist[52] = entity_load(spritelist[20], 1, 1);/*Vbrick270 puzzlethree 1*/
		entitylist[53] = entity_load(spritelist[20], 1, 1);/*Vbrick270 puzzlethree 2*/
		entitylist[54] = entity_load(spritelist[20], 1, 1);/*Vbrick270 puzzlethree 3*/
		entitylist[55] = entity_load(spritelist[20], 1, 1);/*Vbrick270 puzzlethree 4*/
		entitylist[56] = entity_load(spritelist[20], 1, 1);/*Vbrick270 puzzlethree 5*/
		entitylist[57] = entity_load(spritelist[20], 1, 1);/*Vbrick270 puzzlethree 6*/
		entitylist[58] = entity_load(spritelist[22], 1, 1);/*Spike Enemy*/
		entitylist[59] = entity_load(spritelist[9], 1, 1);/*Lever puzzlethree*/
		entitylist[60] = entity_load(spritelist[23], 3, 3);/*eye1 puzzlefour*/
		entitylist[61] = entity_load(spritelist[23], 3, 3);/*eye2 puzzlefour*/
		entitylist[62] = entity_load(spritelist[9], 1, 1);/*Lever puzzlefour*/
		entitylist[63] = entity_load(spritelist[9], 1, 1);/*Lever puzzlefour*/
		entitylist[64] = entity_load(spritelist[9], 1, 1);/*Lever puzzlefour*/
		entitylist[65] = entity_load(spritelist[24], 1, 1);/*You Win*/
		entitylist[66] = entity_load(spritelist[9], 1, 1);/*Lever puzzlethree*/
		entitylist[67] = entity_load(spritelist[9], 1, 1);/*Lever puzzlethree*/

		entitylist[11]->update = doorThink;
		entitylist[12]->update = doorThink;
		entitylist[13]->update = doorThink;
		entitylist[14]->update = doorThink;
		entitylist[15]->update = doorThink;
		entitylist[16]->update = doorThink;

}

void Meter()
{
	if(transCheck == 1)
	{
		if(meterNumber <= 0)
		{
			transCheck = 0;
			meterPower = 0;
			meterNumber = 0;
		}
		else
			meterNumber -= 0.00006;
	}
	else
	{
		if(meterNumber >= 1.5)
		{
			meterPower = 1;
			meterNumber = 1.5;
		}
		else
			meterNumber += 0.00006;
	}
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
	Camera.x = 0;
	gameOver = 0;
	meterNumber = 1.5;

	/*Player Init*/
	entitylist[0]->PositionRect.x = 150;
	entitylist[0]->PositionRect.y = 76;
	entitylist[0]->PositionTemp.x = 150;
	entitylist[0]->PositionTemp.y = 76;
	/*Box Init*/
	entitylist[23]->PositionRect.x = 96;
	entitylist[23]->PositionRect.y = 76;
	/*Door Init*/
	entitylist[11]->PositionRect.y = 450;
	entitylist[16]->PositionRect.y = 450;
	entitylist[12]->PositionRect.y = 482;
	entitylist[13]->PositionRect.y = 482;
	entitylist[14]->PositionRect.y = 482;
	entitylist[15]->PositionRect.y = 482;
	entitylist[11]->health = 1;
	entitylist[16]->health = 1;
	entitylist[12]->health = 1;
	entitylist[13]->health = 1;
	entitylist[14]->health = 1;
	entitylist[15]->health = 1;
	/*Monster Puzzle One Init*/
	entitylist[32]->PositionRect.x = 638;
	entitylist[32]->PositionRect.y = 253;
	entitylist[32]->EventBounds.x = -120;
	entitylist[32]->EventBounds.y = -120;
	entitylist[32]->EventBounds.w = 240;
	entitylist[32]->EventBounds.h = 240;
	entitylist[32]->frame = 0;
	entitylist[32]->dontColl = 1;
	/*Monster 2 and 3*/
	entitylist[7]->PositionRect.x = 405;
	entitylist[7]->PositionRect.y = 84;
	entitylist[8]->PositionRect.x = 714;
	entitylist[8]->PositionRect.y = 292;
	entitylist[7]->frame = 0;
	entitylist[8]->frame = 0;
	entitylist[7]->dontColl = 1;
	entitylist[8]->dontColl = 1;
	/*Monster Puzzle Three Init*/
	entitylist[51]->PositionRect.x = 540;
	entitylist[51]->PositionRect.y = 678;
	entitylist[51]->EventBounds.x = -120;
	entitylist[51]->EventBounds.y = -120;
	entitylist[51]->EventBounds.w = 240;
	entitylist[51]->EventBounds.h = 240;
	entitylist[51]->frame = 0;
	entitylist[51]->dontColl = 1;

	entitylist[58]->PositionRect.x = 540;
	entitylist[58]->PositionRect.y = -578;
	entitylist[58]->frame = 0;
	entitylist[58]->dontColl = 1;
	/*Lever Puzzle One Init*/
	LeverFrame[0] = 0;
	LeverFrame[1] = 0;
	LeverFrame[2] = 0;
	LeverFrame[3] = 0;
	LeverFrame[4] = 0;
	/*Lever Puzzle Two Init*/
	LeverFrame[5] = 0;
	LeverFrame[6] = 0;
	LeverFrame[7] = 0;
	LeverFrame[8] = 0;
	LeverFrame[9] = 0;
	LeverFrame[10] = 0;
	LeverFrame[11] = 0;
	LeverFrame[12] = 0;
	 /*Lever Puzzle 4 Frames*/
	LeverFrame[13] = 0;
	LeverFrame[14] = 0;
	LeverFrame[15] = 0;

	LeverFrame[16] = 0;
	LeverFrame[17] = 0;

	/*Eyes*/
	entitylist[60]->PositionRect.x = 412+1080;
	entitylist[60]->PositionRect.y = entitylist[60]->PositionTemp.y = 20;
	entitylist[61]->PositionRect.x = 668+1080;
	entitylist[61]->PositionRect.y = entitylist[61]->PositionTemp.y = 20;
	entitylist[60]->frame = 0;
	entitylist[61]->frame = 0;
	entitylist[60]->dontColl = 1;
	entitylist[61]->dontColl = 1;

	/*You Win*/
	entitylist[65]->dontColl = 1;

	/*Spikes don't Collide*/
	entitylist[50]->dontColl = 1;

	/*Lever Don't Collide 1 Init*/
	entitylist[9]->dontColl = 1;
	entitylist[59]->dontColl = 1;
	entitylist[33]->dontColl = 1;
	entitylist[34]->dontColl = 1;
	entitylist[35]->dontColl = 1;
	entitylist[36]->dontColl = 1;
	entitylist[37]->dontColl = 1;

	/*Lever Don't Collide 2 Init*/
	entitylist[17]->dontColl = 1;
	entitylist[18]->dontColl = 1;
	entitylist[19]->dontColl = 1;
	entitylist[20]->dontColl = 1;
	entitylist[21]->dontColl = 1;
	entitylist[22]->dontColl = 1;

	entitylist[66]->dontColl = 1;
	entitylist[67]->dontColl = 1;

	/*Lever Puzzle 4*/
	entitylist[62]->dontColl = 1;
	entitylist[63]->dontColl = 1;
	entitylist[64]->dontColl = 1;

	if(level == 2)
	{
		entitylist[32]->PositionRect.x = 1398;
		entitylist[32]->PositionRect.y = 497;
		entitylist[7]->PositionRect.x = 2644;
		entitylist[7]->PositionRect.y = 357;
		entitylist[8]->PositionRect.x = 2135;
		entitylist[8]->PositionRect.y = 127;
	}

}

void puzzleLevelone(int width)
{
	if(transCheck == 0)/*Real World Platforms*/
	{
		levelDraw(backg[0], renderer, Camera);/*Real World Background*/
		entity_draw(entitylist[28], 0, renderer, 32+width, 316, Camera);
		entity_draw(entitylist[26], 0, renderer, 400+width, 108, Camera);
		entity_draw(entitylist[27], 0, renderer, 708+width, 316, Camera);
		entity_draw(entitylist[33], LeverFrame[0], renderer, 430+width, 77, Camera);
		entity_draw(entitylist[34], LeverFrame[1], renderer, 738+width, 285, Camera);
		entity_draw(entitylist[35], LeverFrame[2], renderer, 76+width, 285, Camera);

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
		entity_draw(entitylist[37], LeverFrame[4], renderer, 76+width, 493, Camera);
		entity_draw(entitylist[36], LeverFrame[3], renderer, 500+width, 285, Camera);
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
		entity_draw(entitylist[17], LeverFrame[5], renderer, 156+width, 83, Camera);
		entity_draw(entitylist[48], 0, renderer, 405+width, 324, Camera);
		entity_draw(entitylist[18], LeverFrame[6], renderer, 466+width, 291, Camera);
		entity_draw(entitylist[49], 0, renderer, 714+width, 116, Camera);
		entity_draw(entitylist[19], LeverFrame[7], renderer, 776+width, 83, Camera);

		/*Not Drawn*/
		entitylist[44]->drawn = 0;
		entitylist[45]->drawn = 0;
		entitylist[46]->drawn = 0;
		entitylist[7]->drawn = 0;
		entitylist[8]->drawn = 0;
		entitylist[20]->drawn = 0;
		entitylist[21]->drawn = 0;
		entitylist[22]->drawn = 0;
	}
	else/*Twilight Realms Platforms*/
	{
		entity_draw(entitylist[44], 0, renderer, 96+width, 324, Camera);
		entity_draw(entitylist[20], LeverFrame[8], renderer, 156+width, 291, Camera);
		entity_draw(entitylist[45], 0, renderer, 405+width, 116, Camera);
		entity_draw(entitylist[21], LeverFrame[9], renderer, 466+width, 83, Camera);
		entity_draw(entitylist[46], 0, renderer, 714+width, 324, Camera);
		entity_draw(entitylist[22], LeverFrame[10], renderer, 776+width, 291, Camera);

		entity_draw(entitylist[8], entitylist[8]->frame, renderer, entitylist[8]->PositionRect.x+width, entitylist[8]->PositionRect.y, Camera);

		/*Not Drawn*/
		entitylist[47]->drawn = 0;
		entitylist[48]->drawn = 0;
		entitylist[49]->drawn = 0;
		entitylist[17]->drawn = 0;
		entitylist[18]->drawn = 0;
		entitylist[19]->drawn = 0;

	}

	/*Both Worlds*/
	entity_draw(entitylist[7], entitylist[7]->frame, renderer, entitylist[7]->PositionRect.x+width, entitylist[7]->PositionRect.y, Camera);
	entity_draw(entitylist[2], 0, renderer, 542+width, 450, Camera);/*Platform Door Holder*/
	entity_draw(entitylist[39], 0, renderer, 0+width, 710, Camera);/*Floor*/
	entity_draw(entitylist[31], 0, renderer, 1048+width, 0, Camera);/*Right Side*/
	entity_draw(entitylist[31], 0, renderer, 0+width, 0, Camera);/*Left Side*/
	entity_draw(entitylist[12], 0, renderer, 542+width, entitylist[12]->PositionRect.y, Camera);/*Door One*/
	entity_draw(entitylist[13], 0, renderer, 692+width, entitylist[13]->PositionRect.y, Camera);/*Door Two*/
	entity_draw(entitylist[14], 0, renderer, 842+width, entitylist[14]->PositionRect.y, Camera);/*Door Three*/
	entity_draw(entitylist[15], 0, renderer, 992+width, entitylist[15]->PositionRect.y, Camera);/*Door Four*/
}

void puzzleLevelthree(int width)
{
		if(transCheck == 0)/*Real World Platforms*/
	{
		entity_draw(entitylist[52], 0, renderer, 156+width, 150, Camera);
		entity_draw(entitylist[55], 0, renderer, 156+width, 550, Camera);
		entity_draw(entitylist[56], 0, renderer, 650+width, 150, Camera);
		entity_draw(entitylist[59], LeverFrame[12], renderer, 796+width, 678, Camera);

		if(LeverFrame[11] == 0)
			entity_draw(entitylist[51], entitylist[51]->frame, renderer, entitylist[51]->PositionRect.x+width, entitylist[51]->PositionRect.y, Camera);

		/*Not Drawn*/
		entitylist[58]->drawn = 0;
		entitylist[53]->drawn = 0;
		entitylist[54]->drawn = 0;
		entitylist[57]->drawn = 0;
		entitylist[9]->drawn = 0;

	}
	else/*Twilight Realms Platforms*/
	{

		entity_draw(entitylist[53], 0, renderer, 686+width, 550, Camera);
		entity_draw(entitylist[54], 0, renderer, 386+width, 350, Camera);
		entity_draw(entitylist[57], 0, renderer, 650+width, 150, Camera);
		entity_draw(entitylist[9], LeverFrame[11], renderer, 796+width, 117, Camera);				
		entity_draw(entitylist[67], LeverFrame[17], renderer, 436+width, 317, Camera);
		entity_draw(entitylist[66], LeverFrame[16], renderer, 700+width, 518, Camera);

		if(LeverFrame[11] == 0)
			entity_draw(entitylist[58], entitylist[58]->frame, renderer, entitylist[58]->PositionRect.x+width, entitylist[58]->PositionRect.y, Camera);

		/*Not Drawn*/
		entitylist[52]->drawn = 0;
		entitylist[55]->drawn = 0;
		entitylist[56]->drawn = 0;
		entitylist[51]->drawn = 0;

	}

	/*Both Worlds*/
	entity_draw(entitylist[40], 0, renderer, 0+width, 710, Camera);/*Floor*/
	entity_draw(entitylist[41], 0, renderer, 1048+width, 0, Camera);/*Right Side Platform 450*/
	entity_draw(entitylist[4], 0, renderer, 0+width, 0, Camera);/*Left Side*/
	entity_draw(entitylist[16], 0, renderer, 1048+width, entitylist[16]->PositionRect.y, Camera);/*Door*/
}

void puzzleLevelfour(int width)
{
	if(transCheck == 0)/*Real World Platforms*/
	{
		levelDraw(backg[2], renderer, Camera);/*Real World Background*/
		entity_draw(entitylist[46], 0, renderer, 1040+width, 360, Camera);
		entity_draw(entitylist[48], 0, renderer, 2040+width, 360, Camera);
		entity_draw(entitylist[49], 0, renderer, 140+width, 290, Camera);
		entity_draw(entitylist[54], 0, renderer, 2540+width, 390, Camera);
		entity_draw(entitylist[56], 0, renderer, 1540+width, 560, Camera);

		entity_draw(entitylist[51], entitylist[51]->frame, renderer, entitylist[51]->PositionRect.x+width, entitylist[51]->PositionRect.y, Camera);

		/*Not Drawn*/
		entitylist[60]->drawn = 0;
		entitylist[61]->drawn = 0;
		entitylist[44]->drawn = 0;
		entitylist[45]->drawn = 0;
		entitylist[47]->drawn = 0;
		entitylist[52]->drawn = 0;
		entitylist[55]->drawn = 0;
		entitylist[53]->drawn = 0;
		entitylist[62]->drawn = 0;
		entitylist[64]->drawn = 0;
		entitylist[58]->drawn = 0;
		entitylist[8]->drawn = 0;
		entitylist[32]->drawn = 0;
	}
	else/*Twilight Realms Platforms*/
	{
		levelDraw(backg[3], renderer, Camera);/*Real World Background*/
		entity_draw(entitylist[60], entitylist[60]->frame, renderer, entitylist[60]->PositionRect.x+width, entitylist[60]->PositionRect.y, Camera);
		entity_draw(entitylist[61], entitylist[61]->frame, renderer, entitylist[61]->PositionRect.x+width, entitylist[61]->PositionRect.y, Camera);

		if(gameOver == 1)
			entity_draw(entitylist[65], 0, renderer, 1440, 160, Camera);

		entity_draw(entitylist[58], entitylist[58]->frame, renderer, entitylist[58]->PositionRect.x+width, entitylist[58]->PositionRect.y, Camera);
		entity_draw(entitylist[32], entitylist[32]->frame, renderer, entitylist[32]->PositionRect.x+width, entitylist[32]->PositionRect.y, Camera);/*Monster*/
		entity_draw(entitylist[8], entitylist[8]->frame, renderer, entitylist[8]->PositionRect.x+width, entitylist[8]->PositionRect.y, Camera);/*Monster*/

		entity_draw(entitylist[44], 0, renderer, 240+width, 360, Camera);
		entity_draw(entitylist[45], 0, renderer, 1240+width, 560, Camera);
		entity_draw(entitylist[47], 0, renderer, 1040+width, 160, Camera);/*Lever Platform*/
		entity_draw(entitylist[62], LeverFrame[13], renderer, 1240+width, 128, Camera);/*Lever*/
		entity_draw(entitylist[52], 0, renderer, 2040+width, 160, Camera);/*Lever Platform*/
		entity_draw(entitylist[63], LeverFrame[14], renderer, 2080+width, 128, Camera);/*Lever*/
		entity_draw(entitylist[53], 0, renderer, 2240+width, 560, Camera);
		entity_draw(entitylist[55], 0, renderer, 2940+width, 190, Camera);/*Lever Platform*/
		entity_draw(entitylist[64], LeverFrame[15], renderer, 3180+width, 158, Camera);/*Lever*/
		

		/*Not Drawn*/
		entitylist[46]->drawn = 0;
		entitylist[48]->drawn = 0;
		entitylist[49]->drawn = 0;
		entitylist[54]->drawn = 0;
		entitylist[51]->drawn = 0;
		entitylist[56]->drawn = 0;
	}

	/*Both Worlds*/
	entity_draw(entitylist[7], entitylist[7]->frame, renderer, entitylist[7]->PositionRect.x+width, entitylist[7]->PositionRect.y, Camera);
	entity_draw(entitylist[38], 0, renderer, 1080+width, 710, Camera);/*Floor*/
	entity_draw(entitylist[39], 0, renderer, 2160+width, 710, Camera);/*Floor*/
	entity_draw(entitylist[40], 0, renderer, 0+width, 710, Camera);/*Floor*/
	entity_draw(entitylist[42], 0, renderer, 3208+width, 0, Camera);/*Right Side*/
	entity_draw(entitylist[4], 0, renderer, 0+width, 0, Camera);/*Left Side*/

}

void ChangeWorlds()
{
	if(entitylist[0]->PositionRect.x > 3240)
	{
		level = 2;
		InitPos();
		entity_drawn_free();
	}
}

void createEntity(int i, int x, int y)
{
	int a;

	if(transCheck == 0)
	{
		for(a = 68; a<=76; a++)
		{
			if(entitylist[a] == NULL)
			{
				entitylist[a] = entity_load(spritelist[i], 1, 1);
				entitylist[a]->PositionRect.x = x;
				entitylist[a]->PositionRect.y = y;
				entitylist[a]->PositionTemp.x = x;
				entitylist[a]->PositionTemp.y = y;
				entitylist[a]->frame = 0;

				if(entitylist[i]->sprite == spritelist[11])
				{
					entitylist[a]->update = doorThink;
				}
				
				if(entitylist[i]->sprite == spritelist[9])
				{
					entitylist[a]->update = leverThink;
					entitylist[a]->touch = OpenDoor;
					entitylist[a]->dontColl = 1;
				}
				break;
			}
		}
	}
	else if(transCheck == 1) 
	{
		for(a = 77; a<=85; a++)
		{
			if(entitylist[a] == NULL)
			{
				entitylist[a] = entity_load(spritelist[i], 1, 1);
				entitylist[a]->PositionRect.x = x;
				entitylist[a]->PositionRect.y = y;
				entitylist[a]->PositionTemp.x = x;
				entitylist[a]->PositionTemp.y = y;
				entitylist[a]->frame = 0;

				if(entitylist[i]->sprite == spritelist[11])
				{
					entitylist[a]->update = doorThink;
				}

				if(entitylist[i]->sprite == spritelist[9])
				{
					entitylist[a]->update = leverThink;
					entitylist[a]->touch = OpenDoor;
					entitylist[a]->dontColl = 1;
				}
				break;
			}
		}
	}
	else if(transCheck == 2)
	{
		for(a = 86; a<=94; a++)
		{
			if(entitylist[a] == NULL)
			{
				entitylist[a] = entity_load(spritelist[i], 1, 1);
				entitylist[a]->PositionRect.x = x;
				entitylist[a]->PositionRect.y = y;
				entitylist[a]->PositionTemp.x = x;
				entitylist[a]->PositionTemp.y = y;
				entitylist[a]->frame = 0;

				if(entitylist[i]->sprite == spritelist[11])
				{
					entitylist[a]->update = doorThink;
				}

				if(entitylist[i]->sprite == spritelist[9])
				{
					entitylist[a]->update = leverThink;
					entitylist[a]->touch = OpenDoor;
					entitylist[a]->dontColl = 1;
				}
				break;
			}
		}
	}

}

void editor()
{
	if(transCheck == 0)/*Real World Platforms*/
	{
		levelDraw(backg[0], renderer, Camera);/*Real World Background*/

		if(entitylist[68] != NULL)
			entity_draw(entitylist[68], entitylist[68]->frame, renderer, entitylist[68]->PositionRect.x, entitylist[68]->PositionRect.y, Camera);
		if(entitylist[69] != NULL)
			entity_draw(entitylist[69], entitylist[69]->frame, renderer, entitylist[69]->PositionRect.x, entitylist[69]->PositionRect.y, Camera);
		if(entitylist[70] != NULL)
			entity_draw(entitylist[70], entitylist[70]->frame, renderer, entitylist[70]->PositionRect.x, entitylist[70]->PositionRect.y, Camera);
		if(entitylist[71] != NULL)
			entity_draw(entitylist[71], entitylist[71]->frame, renderer, entitylist[71]->PositionRect.x, entitylist[71]->PositionRect.y, Camera);
		if(entitylist[72] != NULL)
			entity_draw(entitylist[72], entitylist[72]->frame, renderer, entitylist[72]->PositionRect.x, entitylist[72]->PositionRect.y, Camera);
		if(entitylist[73] != NULL)
			entity_draw(entitylist[73], entitylist[73]->frame, renderer, entitylist[73]->PositionRect.x, entitylist[73]->PositionRect.y, Camera);
		if(entitylist[74] != NULL)
			entity_draw(entitylist[74], entitylist[74]->frame, renderer, entitylist[74]->PositionRect.x, entitylist[74]->PositionRect.y, Camera);
		if(entitylist[75] != NULL)
			entity_draw(entitylist[75], entitylist[75]->frame, renderer, entitylist[75]->PositionRect.x, entitylist[75]->PositionRect.y, Camera);
		if(entitylist[76] != NULL)
			entity_draw(entitylist[76], entitylist[76]->frame, renderer, entitylist[76]->PositionRect.x, entitylist[76]->PositionRect.y, Camera);

		/*Not Drawn*/
		if(entitylist[77] != NULL)
			entitylist[77]->drawn = 0;
		if(entitylist[78] != NULL)
			entitylist[78]->drawn = 0;
		if(entitylist[79] != NULL)
			entitylist[79]->drawn = 0;
		if(entitylist[80] != NULL)
			entitylist[80]->drawn = 0;
		if(entitylist[81] != NULL)
			entitylist[81]->drawn = 0;
		if(entitylist[82] != NULL)
			entitylist[82]->drawn = 0;
		if(entitylist[83] != NULL)
			entitylist[83]->drawn = 0;
		if(entitylist[84] != NULL)
			entitylist[84]->drawn = 0;
		if(entitylist[85] != NULL)
			entitylist[85]->drawn = 0;

	}
	else/*Twilight Realms Platforms*/
	{
		levelDraw(backg[1], renderer, Camera);/*Twilight World Background*/

		if(entitylist[77] != NULL)
			entity_draw(entitylist[77], entitylist[77]->frame, renderer, entitylist[77]->PositionRect.x, entitylist[77]->PositionRect.y, Camera);
		if(entitylist[78] != NULL)
			entity_draw(entitylist[78], entitylist[78]->frame, renderer, entitylist[78]->PositionRect.x, entitylist[78]->PositionRect.y, Camera);
		if(entitylist[79] != NULL)
			entity_draw(entitylist[79], entitylist[79]->frame, renderer, entitylist[79]->PositionRect.x, entitylist[79]->PositionRect.y, Camera);
		if(entitylist[80] != NULL)
			entity_draw(entitylist[80], entitylist[80]->frame, renderer, entitylist[80]->PositionRect.x, entitylist[80]->PositionRect.y, Camera);
		if(entitylist[81] != NULL)
			entity_draw(entitylist[81], entitylist[81]->frame, renderer, entitylist[81]->PositionRect.x, entitylist[81]->PositionRect.y, Camera);
		if(entitylist[82] != NULL)
			entity_draw(entitylist[82], entitylist[82]->frame, renderer, entitylist[82]->PositionRect.x, entitylist[82]->PositionRect.y, Camera);
		if(entitylist[83] != NULL)
			entity_draw(entitylist[83], entitylist[83]->frame, renderer, entitylist[83]->PositionRect.x, entitylist[83]->PositionRect.y, Camera);
		if(entitylist[84] != NULL)
			entity_draw(entitylist[84], entitylist[84]->frame, renderer, entitylist[84]->PositionRect.x, entitylist[84]->PositionRect.y, Camera);
		if(entitylist[85] != NULL)
			entity_draw(entitylist[85], entitylist[85]->frame, renderer, entitylist[85]->PositionRect.x, entitylist[85]->PositionRect.y, Camera);

		/*Not Drawn*/
		if(entitylist[68] != NULL)
			entitylist[68]->drawn = 0;
		if(entitylist[69] != NULL)
			entitylist[69]->drawn = 0;
		if(entitylist[70] != NULL)
			entitylist[70]->drawn = 0;
		if(entitylist[71] != NULL)
			entitylist[71]->drawn = 0;
		if(entitylist[72] != NULL)
			entitylist[72]->drawn = 0;
		if(entitylist[73] != NULL)
			entitylist[73]->drawn = 0;
		if(entitylist[74] != NULL)
			entitylist[74]->drawn = 0;
		if(entitylist[75] != NULL)
			entitylist[75]->drawn = 0;
		if(entitylist[76] != NULL)
			entitylist[76]->drawn = 0;

	}

	/*Both Worlds*/
		entity_draw(entitylist[38], 0, renderer, 1080, 710, Camera);/*Floor*/
		entity_draw(entitylist[39], 0, renderer, 2160, 710, Camera);/*Floor*/
		entity_draw(entitylist[40], 0, renderer, 0, 710, Camera);/*Floor*/

		if(entitylist[86] != NULL)
			entity_draw(entitylist[86], entitylist[86]->frame, renderer, entitylist[86]->PositionRect.x, entitylist[86]->PositionRect.y, Camera);
		if(entitylist[87] != NULL)
			entity_draw(entitylist[87], entitylist[87]->frame, renderer, entitylist[87]->PositionRect.x, entitylist[87]->PositionRect.y, Camera);
		if(entitylist[88] != NULL)
			entity_draw(entitylist[88], entitylist[88]->frame, renderer, entitylist[88]->PositionRect.x, entitylist[88]->PositionRect.y, Camera);
		if(entitylist[89] != NULL)
			entity_draw(entitylist[89], entitylist[89]->frame, renderer, entitylist[89]->PositionRect.x, entitylist[89]->PositionRect.y, Camera);
		if(entitylist[90] != NULL)
			entity_draw(entitylist[90], entitylist[90]->frame, renderer, entitylist[90]->PositionRect.x, entitylist[90]->PositionRect.y, Camera);
		if(entitylist[91] != NULL)
			entity_draw(entitylist[91], entitylist[91]->frame, renderer, entitylist[91]->PositionRect.x, entitylist[91]->PositionRect.y, Camera);
		if(entitylist[92] != NULL)
			entity_draw(entitylist[92], entitylist[92]->frame, renderer, entitylist[92]->PositionRect.x, entitylist[92]->PositionRect.y, Camera);
		if(entitylist[93] != NULL)
			entity_draw(entitylist[93], entitylist[93]->frame, renderer, entitylist[93]->PositionRect.x, entitylist[93]->PositionRect.y, Camera);
		if(entitylist[94] != NULL)
			entity_draw(entitylist[94], entitylist[94]->frame, renderer, entitylist[94]->PositionRect.x, entitylist[94]->PositionRect.y, Camera);

}

void LevelOrder()
{
		/*LEVEL 1*/
	if(level == 1)
	{
		textChange(fontList[12], "World 1", renderer);
		monsterInfo(entitylist[32]);
		monsterInfo2(405, 650, 0);
		monsterInfo3(0);
		monsterInfo4(96, 954, 0, 365);

		puzzleLevelone(1080);/*Puzzle One*/
		puzzleLeveltwo(2160);/*Puzzle Two*/
		puzzleLevelthree(0);
	}
	else
	{
		/*Level 2*/
		textChange(fontList[12], "World 2", renderer);
		monsterInfo(entitylist[32]);
		monsterInfo2(2506, 2784, 0);
		monsterInfo3(0);
		monsterInfo4(1027, 2287, 0, 700);
		boss();
		puzzleLevelfour(0);
	}
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

void monsterInfo(Entity_t *a)
{
	/*Monster*/
			if(entity_intersect_event_all(entitylist[0]))
			{
				/*Monster Animation*/
 				sprite_animation(8, 0, 32.5f, frameTime, a->frame);
			}
			else
				a->frame = 0;
}

void monsterInfo2(int leftMax, int rightMax, int width)
{

	/*Monster*/

			if(slowTime <= 75)
				entitylist[7]->frame = 8;
			else
				entitylist[7]->frame = 0;
				 

			if(moveright == 1)
			{
				slowTime++;

				if(slowTime == 150)
				{
					entitylist[7]->PositionRect.x += 5;
					slowTime = 0;
				}

				if(entitylist[7]->PositionRect.x >= rightMax+width)
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

				if(entitylist[7]->PositionRect.x <= leftMax+width)
				{
					moveleft = 0;
					moveright = 1;
				}
			}

}

void monsterInfo3(int width)
{
	int movespeed = 1;

	if(LeverFrame[16] == 1)
		movespeed = 3;

	if((transCheck == 1 && LeverFrame[11] == 0) || (transCheck == 1 && level == 2))
		Fall(entitylist[58]->PositionRect, entitylist[58]->PositionTemp, deltaTime, entitylist[51], movespeed);
	else
	{
		entitylist[58]->PositionRect.y = -578;
		entitylist[51]->PositionRect.x = entitylist[0]->PositionRect.x - 100;
	}

	if(entitylist[58]->PositionRect.y >= 1180)
	{
		entitylist[58]->PositionRect.y = -578;
		entitylist[58]->PositionRect.x = entitylist[0]->PositionRect.x - 100;
	}

	if(slowTime3 == 1950)
	{	
		entitylist[51]->frame++;
		entitylist[58]->frame++;
		slowTime3 = 0;
	}
	else
	{
		slowTime3++;
	}

	if(entitylist[51]->frame == 8 || entitylist[58]->frame == 8)
		entitylist[51]->frame = entitylist[58]->frame = 0;

}

void monsterInfo4(int leftMax, int rightMax, int width, int teleDistance)
{
		if(moveright2 == 1)
			{
				slowTime2++;

				if(slowTime2 == 150)
				{
					entitylist[8]->PositionRect.x += 5;
					slowTime2 = 0;
				}

				if(entitylist[8]->PositionRect.x <= leftMax+270-7+width && entitylist[8]->PositionRect.x >= leftMax+270-15+width)
					entitylist[8]->PositionRect.x += teleDistance;

				if(entitylist[8]->PositionRect.x >= rightMax+width)
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

				if(entitylist[8]->PositionRect.x <= teleDistance+leftMax+270-15+width && entitylist[8]->PositionRect.x >= teleDistance+leftMax+270-22+width)
					entitylist[8]->PositionRect.x -= teleDistance;

				if(entitylist[8]->PositionRect.x <= leftMax+width)
				{
					moveleft2 = 0;
					moveright2 = 1;
				}
			}

}

void leverAction()
{
					if(entity_return_intersect_all(entitylist[0]) == entitylist[66])
					{
						Mix_PlayChannel(-1, soundList[0]->seffect, 0);
							LeverFrame[16] = 1;
					}
					if(entity_return_intersect_all(entitylist[0]) == entitylist[67])
					{
						Mix_PlayChannel(-1, soundList[0]->seffect, 0);
							LeverFrame[17] = 1;
					}
					if(entity_return_intersect_all(entitylist[0]) == entitylist[64])
					{
					Mix_PlayChannel(-1, soundList[0]->seffect, 0);
							LeverFrame[15] = 1;
					}
					if(entity_return_intersect_all(entitylist[0]) == entitylist[63])
					{
					Mix_PlayChannel(-1, soundList[0]->seffect, 0);
							LeverFrame[14] = 1;
					}
					if(entity_return_intersect_all(entitylist[0]) == entitylist[62])
					{
					Mix_PlayChannel(-1, soundList[0]->seffect, 0);
							LeverFrame[13] = 1;
					}
					if(entity_return_intersect_all(entitylist[0]) == entitylist[9])
					{
					Mix_PlayChannel(-1, soundList[0]->seffect, 0);
							LeverFrame[11] = 1;
					}
					if(entity_return_intersect_all(entitylist[0]) == entitylist[59])
					{
					Mix_PlayChannel(-1, soundList[0]->seffect, 0);
							LeverFrame[12] = 1;
							entitylist[16]->health = 0;
					}
					if(entity_return_intersect_all(entitylist[0]) == entitylist[33])
					{
					Mix_PlayChannel(-1, soundList[0]->seffect, 0);
							LeverFrame[0] = 1;
							leverorder[i] = 1;
							i++;
					}

					if(entity_return_intersect_all(entitylist[0]) == entitylist[34])
					{
					Mix_PlayChannel(-1, soundList[0]->seffect, 0);
							LeverFrame[1] = 1;
							leverorder[i] = 2;
							i++;
					}

					if(entity_return_intersect_all(entitylist[0]) == entitylist[35])
					{
					Mix_PlayChannel(-1, soundList[0]->seffect, 0);
							LeverFrame[2] = 1;
							leverorder[i] = 3;
							i++;
					}

					if(entity_return_intersect_all(entitylist[0]) == entitylist[36])
					{
					Mix_PlayChannel(-1, soundList[0]->seffect, 0);
							LeverFrame[3] = 1;
							leverorder[i] = 4;
							i++;
					}

					if(entity_return_intersect_all(entitylist[0]) == entitylist[37])
					{
					Mix_PlayChannel(-1, soundList[0]->seffect, 0);
							LeverFrame[4] = 1;
							leverorder[i] = 5;
							i++;
					}
			
					actionCheck = 0;

					if(puzzleoneInfo() && i == 5)
						entitylist[11]->health = 0;
					else if(i == 5)
					{
					Mix_PlayChannel(-1, soundList[0]->seffect, 0);
						LeverFrame[0] = 0;
						LeverFrame[1] = 0;
						LeverFrame[2] = 0;
						LeverFrame[3] = 0;
						LeverFrame[4] = 0;
						i = 0;
					}



					/*Lever Puzzle Two*/

					if(entity_return_intersect_all(entitylist[0]) == entitylist[17])
					{

					Mix_PlayChannel(-1, soundList[0]->seffect, 0);

							if(LeverFrame[5] == 1)
							{
								LeverFrame[5] = 0;
								entitylist[12]->health = 1;
								entitylist[14]->health = 1;
							}
							else
							{
								LeverFrame[5] = 1;
								entitylist[12]->health = 0;
								entitylist[14]->health = 0;
							}
					}

					if(entity_return_intersect_all(entitylist[0]) == entitylist[18])
					{
						
					Mix_PlayChannel(-1, soundList[0]->seffect, 0);

						  if(LeverFrame[6] == 1)
							{
								LeverFrame[6] = 0;
								entitylist[13]->health = 1;
								entitylist[15]->health = 0;
							}
						  else
						  {
							LeverFrame[6] = 1;
								entitylist[13]->health = 0;
								entitylist[15]->health = 1;
						  }
					}

					if(entity_return_intersect_all(entitylist[0]) == entitylist[19])
					{
						
					Mix_PlayChannel(-1, soundList[0]->seffect, 0);

						if(LeverFrame[7] == 1)
							{
								LeverFrame[7] = 0;
								entitylist[13]->health = 1;
								entitylist[15]->health = 0;
							}
						else
						{
							LeverFrame[7] = 1;
								entitylist[14]->health = 0;
								entitylist[13]->health = 1;
						}
					}

					if(entity_return_intersect_all(entitylist[0]) == entitylist[20])
					{		
						
					Mix_PlayChannel(-1, soundList[0]->seffect, 0);

						if(LeverFrame[8] == 1)
							{
								LeverFrame[8] = 0;
								entitylist[13]->health = 1;
								entitylist[14]->health = 0;
							}
						else
						{
							LeverFrame[8] = 1;
								entitylist[13]->health = 0;
								entitylist[14]->health = 1;
						}
					}

					if(entity_return_intersect_all(entitylist[0]) == entitylist[21])
					{
						
					Mix_PlayChannel(-1, soundList[0]->seffect, 0);

						if(LeverFrame[9] == 1)
							{
								LeverFrame[9] = 0;
								entitylist[15]->health = 1;
								entitylist[12]->health = 0;
							}
						else
						{
							LeverFrame[9] = 1;
								entitylist[15]->health = 0;
								entitylist[12]->health = 1;
						}
					}
					if(entity_return_intersect_all(entitylist[0]) == entitylist[22])
					{
						
					Mix_PlayChannel(-1, soundList[0]->seffect, 0);

						if(LeverFrame[10] == 1)
							{
								LeverFrame[10] = 0;
								entitylist[12]->health = 1;
								entitylist[13]->health = 0;
							}
						else
						{
							LeverFrame[10] = 1;
								entitylist[12]->health = 0;
								entitylist[13]->health = 1;
						}
					}

					if(entity_return_intersect_all(entitylist[0])->sprite == spritelist[9] && entity_return_intersect_all(entitylist[0])->update != NULL && entity_return_intersect_all(entitylist[0])->health == 1)
					{
						entity_return_intersect_all(entitylist[0])->health = 0;
						entity_return_intersect_all(entitylist[0])->update(entity_return_intersect_all(entitylist[0]));
						entity_return_intersect_all(entitylist[0])->touch(entity_return_intersect_all(entitylist[0]), entity_find(spritelist[11]));
					}

}

void boss()
{
	if(LeverFrame[13] == 1 && LeverFrame[14] == 1 && LeverFrame[15] == 1)
	{
		entitylist[60]->health = 0;

		gameOver = 1;
		entitylist[0]->drawn = 0;
		entitylist[0]->PositionRect.x = 1100+540;
		entitylist[0]->PositionRect.y = 656;
		Camera.x = 1100;

		if(slowTime < 44)
		{
				entitylist[60]->frame = 0;
				entitylist[61]->frame = 0;
				gameOverTime++;
		}
		else if(slowTime < 99)
		{
				entitylist[60]->frame = 1;
				entitylist[61]->frame = 1;
		}
		else
				entitylist[60]->frame = 2;
				entitylist[61]->frame = 2;
		
		if(gameOverTime > 1500)
		{
			Fall(entitylist[60]->PositionRect, entitylist[60]->PositionTemp, deltaTime, entitylist[60], 1);
			Fall(entitylist[61]->PositionRect, entitylist[61]->PositionTemp, deltaTime, entitylist[61], 1);
		}

	}

	if(entitylist[0]->PositionRect.x < 1300 && gameOver == 0)
	{
		entitylist[60]->frame = 0;
		entitylist[61]->frame = 0;
	}
	else if(entitylist[0]->PositionRect.x < 2000 && gameOver == 0)
	{
		entitylist[60]->frame = 1;
		entitylist[61]->frame = 1;
	}
	else if(gameOver == 0)
	{
		entitylist[60]->frame = 2;
		entitylist[61]->frame = 2;
	}

}

void editorSave()
{
	int a;

	/*Real World Entity Save*/
		for(a = 68; a<=76; a++)
		{
			if(entitylist[a] != NULL)
			{
				saveContents(a, "levels/level1.txt");
				saveContents(entitylist[a]->sprite->ID, "levels/level1.txt");
				saveContents(entitylist[a]->PositionRect.x, "levels/level1.txt");
				saveContents(entitylist[a]->PositionRect.y, "levels/level1.txt");
				saveContents(entitylist[a]->PositionTemp.x, "levels/level1.txt");
				saveContents(entitylist[a]->PositionTemp.y, "levels/level1.txt");
				saveContents(entitylist[a]->frame, "levels/level1.txt");
				
				if(entitylist[a]->sprite == spritelist[9])
				{
					saveContents(entitylist[a]->dontColl, "levels/level1.txt");
				}

			}
			else
			{
				saveContents(888888, "levels/level1.txt");
				break;
			}
		}

	/*Twilight World Entity Save*/
		for(a = 77; a<=85; a++)
		{
			if(entitylist[a] != NULL)
			{
				saveContents(a, "levels/level1.txt");
				saveContents(entitylist[a]->sprite->ID, "levels/level1.txt");
				saveContents(entitylist[a]->PositionRect.x, "levels/level1.txt");
				saveContents(entitylist[a]->PositionRect.y, "levels/level1.txt");
				saveContents(entitylist[a]->PositionTemp.x, "levels/level1.txt");
				saveContents(entitylist[a]->PositionTemp.y, "levels/level1.txt");
				saveContents(entitylist[a]->frame, "levels/level1.txt");

				if(entitylist[a]->sprite == spritelist[9])
				{
					saveContents(entitylist[a]->dontColl, "levels/level1.txt");
				}
			}
			else
			{
				saveContents(888888, "levels/level1.txt");
				break;
			}
		}

	/*Both World Entity Save*/
		for(a = 86; a<=94; a++)
		{
			if(entitylist[a] != NULL)
			{
				saveContents(a, "levels/level1.txt");
				saveContents(entitylist[a]->sprite->ID, "levels/level1.txt");
				saveContents(entitylist[a]->PositionRect.x, "levels/level1.txt");
				saveContents(entitylist[a]->PositionRect.y, "levels/level1.txt");
				saveContents(entitylist[a]->PositionTemp.x, "levels/level1.txt");
				saveContents(entitylist[a]->PositionTemp.y, "levels/level1.txt");
				saveContents(entitylist[a]->frame, "levels/level1.txt");
				
				if(entitylist[a]->sprite == spritelist[9])
				{
					saveContents(entitylist[a]->dontColl, "levels/level1.txt");
				}
			}
			else
			{
				saveContents(888888, "levels/level1.txt");
				break;
			}
		}

		saveContents(999999, "levels/level1.txt");
}
void editorLoad()
{
	int a;

		for(a = 68; a<=76; a++)
		{
			if(entitylist[a] == NULL)
			{
				if(loadContents("levels/level1.txt") != a)
					break;

				spriteLoad = loadContents("levels/level1.txt");
				entitylist[a] = entity_load(spritelist[spriteLoad], 1, 1);
				entitylist[a]->PositionRect.x = loadContents("levels/level1.txt");
				entitylist[a]->PositionRect.y = loadContents("levels/level1.txt");
				entitylist[a]->PositionTemp.x = loadContents("levels/level1.txt");
				entitylist[a]->PositionTemp.y = loadContents("levels/level1.txt");
				entitylist[a]->frame = loadContents("levels/level1.txt");

				if(entitylist[a]->sprite == spritelist[11])
				{
					entitylist[a]->update = doorThink;
				}
				
				if(entitylist[a]->sprite == spritelist[9])
				{
					entitylist[a]->update = leverThink;
					entitylist[a]->touch = OpenDoor;
					entitylist[a]->dontColl = loadContents("levels/level1.txt");
				}

			}

		}

		for(a = 77; a<=85; a++)
		{
			if(entitylist[a] == NULL)
			{
				if(loadContents("levels/level1.txt") != a)
					break;

				spriteLoad = loadContents("levels/level1.txt");
				entitylist[a] = entity_load(spritelist[spriteLoad], 1, 1);
				entitylist[a]->PositionRect.x = loadContents("levels/level1.txt");
				entitylist[a]->PositionRect.y = loadContents("levels/level1.txt");
				entitylist[a]->PositionTemp.x = loadContents("levels/level1.txt");
				entitylist[a]->PositionTemp.y = loadContents("levels/level1.txt");
				entitylist[a]->frame = loadContents("levels/level1.txt");

				if(entitylist[a]->sprite == spritelist[11])
				{
					entitylist[a]->update = doorThink;
				}
				
				if(entitylist[a]->sprite == spritelist[9])
				{
					entitylist[a]->update = leverThink;
					entitylist[a]->touch = OpenDoor;
					entitylist[a]->dontColl = loadContents("levels/level1.txt");
				}

			}
		}

		for(a = 86; a<=94; a++)
		{
			if(entitylist[a] == NULL)
			{
				if(loadContents("levels/level1.txt") != a)
					break;

				spriteLoad = loadContents("levels/level1.txt");
				entitylist[a] = entity_load(spritelist[spriteLoad], 1, 1);
				entitylist[a]->PositionRect.x = loadContents("levels/level1.txt");
				entitylist[a]->PositionRect.y = loadContents("levels/level1.txt");
				entitylist[a]->PositionTemp.x = loadContents("levels/level1.txt");
				entitylist[a]->PositionTemp.y = loadContents("levels/level1.txt");
				entitylist[a]->frame = loadContents("levels/level1.txt");

				if(entitylist[a]->sprite == spritelist[11])
				{
					entitylist[a]->update = doorThink;
				}
				
				if(entitylist[a]->sprite == spritelist[9])
				{
					entitylist[a]->update = leverThink;
					entitylist[a]->touch = OpenDoor;
					entitylist[a]->dontColl = loadContents("levels/level1.txt");
				}

			}
		}

	contentEnder = loadContents("levels/level1.txt");

}
void save()
{
	saveContents(LeverFrame[0] ,"savegames/save1.txt");
	saveContents(LeverFrame[1] ,"savegames/save1.txt");
	saveContents(LeverFrame[2] ,"savegames/save1.txt");
	saveContents(LeverFrame[3] ,"savegames/save1.txt");
	saveContents(LeverFrame[4] ,"savegames/save1.txt");
	saveContents(LeverFrame[5] ,"savegames/save1.txt");
	saveContents(LeverFrame[6] ,"savegames/save1.txt");
	saveContents(LeverFrame[7] ,"savegames/save1.txt");
	saveContents(LeverFrame[8] ,"savegames/save1.txt");
	saveContents(LeverFrame[9] ,"savegames/save1.txt");
	saveContents(LeverFrame[10] ,"savegames/save1.txt");
	saveContents(LeverFrame[11] ,"savegames/save1.txt");
	saveContents(LeverFrame[12] ,"savegames/save1.txt");
	saveContents(LeverFrame[13] ,"savegames/save1.txt");
	saveContents(LeverFrame[14] ,"savegames/save1.txt");
	saveContents(LeverFrame[15] ,"savegames/save1.txt");
	saveContents(LeverFrame[16] ,"savegames/save1.txt");
	saveContents(LeverFrame[17] ,"savegames/save1.txt");
	saveContents(entitylist[11]->health,"savegames/save1.txt");
	saveContents(entitylist[12]->health,"savegames/save1.txt");
	saveContents(entitylist[13]->health,"savegames/save1.txt");
	saveContents(entitylist[14]->health,"savegames/save1.txt");
	saveContents(entitylist[15]->health,"savegames/save1.txt");
	saveContents(entitylist[16]->health,"savegames/save1.txt");
	saveContents(entitylist[0]->PositionRect.x,"savegames/save1.txt");
	saveContents(entitylist[0]->PositionRect.y,"savegames/save1.txt");
	saveContents(entitylist[0]->PositionTemp.x,"savegames/save1.txt");
	saveContents(entitylist[0]->PositionTemp.y,"savegames/save1.txt");
	saveContents(999999,"savegames/save1.txt");
}

void load()
{
	LeverFrame[0] = loadContents("savegames/save1.txt");
	LeverFrame[1] = loadContents("savegames/save1.txt");
	LeverFrame[2] = loadContents("savegames/save1.txt");
	LeverFrame[3] = loadContents("savegames/save1.txt");
	LeverFrame[4] = loadContents("savegames/save1.txt");
	LeverFrame[5] = loadContents("savegames/save1.txt");
	LeverFrame[6] = loadContents("savegames/save1.txt");
	LeverFrame[7] = loadContents("savegames/save1.txt");
	LeverFrame[8] = loadContents("savegames/save1.txt");
	LeverFrame[9] = loadContents("savegames/save1.txt");
	LeverFrame[10] = loadContents("savegames/save1.txt");
	LeverFrame[11] = loadContents("savegames/save1.txt");
	LeverFrame[12] = loadContents("savegames/save1.txt");
	LeverFrame[13] = loadContents("savegames/save1.txt");
	LeverFrame[14] = loadContents("savegames/save1.txt");
	LeverFrame[15] = loadContents("savegames/save1.txt");
	LeverFrame[16] = loadContents("savegames/save1.txt");
	LeverFrame[17] = loadContents("savegames/save1.txt");
	entitylist[11]->health = loadContents("savegames/save1.txt");
	entitylist[12]->health = loadContents("savegames/save1.txt");
	entitylist[13]->health = loadContents("savegames/save1.txt");
	entitylist[14]->health = loadContents("savegames/save1.txt");
	entitylist[15]->health = loadContents("savegames/save1.txt");
	entitylist[16]->health = loadContents("savegames/save1.txt");
	entitylist[0]->PositionRect.x = loadContents("savegames/save1.txt");
	entitylist[0]->PositionRect.y = loadContents("savegames/save1.txt");
	entitylist[0]->PositionTemp.x = loadContents("savegames/save1.txt");
	entitylist[0]->PositionTemp.x = loadContents("savegames/save1.txt");
	contentEnder = loadContents("savegames/save1.txt");
	
}