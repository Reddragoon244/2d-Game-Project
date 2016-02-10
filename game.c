#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "sprite.h"
#include <string>

extern SDL_Surface *screen;
extern SDL_Surface *buffer;



int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;//always make a poiter null at initial
	SDL_Surface *temp = NULL;
	bool isRunning = true;
	SDL_Event ev;


		window = SDL_CreateWindow("Into the Twilight", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 640, SDL_WINDOW_SHOWN);

			while(isRunning)
			{
				while(SDL_PollEvent(&ev) != 0)
				{
					if(ev.type == SDL_QUIT)
						isRunning = false;
				}

				SDL_UpdateWindowSurface(window);
			}
		
	
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
















