#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "sprite.h"
#include <string>

SDL_Renderer *renderer;
SDL_Texture *texture;

SDL_Texture *LoadTexture(char *filename, SDL_Renderer *rendererTarget)
{
	SDL_Texture *texture = NULL;
	SDL_Surface *surface = SDL_LoadBMP(filename);
	if(surface == NULL)
		printf("failed to get surface: %s", SDL_GetError());
	else
	{
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		if(texture == NULL)
			printf("failed to create texture: %s", SDL_GetError());

	}

	SDL_FreeSurface(surface);

	return texture;
}

int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;//always make a poiter null at initial
	SDL_Surface *temp = NULL;
	SDL_Rect playerRect, playerPos;
	playerPos.x = playerPos.y = 0;//position of image
	playerPos.w = playerPos.h = 64;//size of the image on screen
	int textureW, textureH;
	int frameW, frameH, FPS = 60, frameTime = 0;
	bool isRunning = true;
	SDL_Event ev;


		window = SDL_CreateWindow("Into the Twilight", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 720, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
		texture = LoadTexture("image.bmp", renderer);/*change this so LoadTexture is a pointer to a SDL_Texture*/

		initSpriteSystem();
		sprite_load("image.bmp", 13, 21);

		SDL_QueryTexture(texture, NULL, NULL, &textureW, &textureH);
		frameW = textureW / 13;
		frameH = textureH / 21;

		playerRect.x = playerRect.y = 0;
		playerRect.w = frameW;
		playerRect.h = frameH;

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
					playerRect.x += frameW;
					if(playerRect.x >= textureW)
						playerRect.x = 0;
				}

				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, texture, &playerRect, &playerPos);
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
















