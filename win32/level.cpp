#include <stdlib.h>
#include <string>
#include "level.h"

Level level;

Level *levelload(char *filepath, SDL_Renderer *renderer)
{
	SDL_Texture *texture = NULL;
	SDL_Surface *surface = IMG_Load(filepath);

	if(surface == NULL)
	  fprintf(stderr, "Error surface null", SDL_GetError());
	else
	{
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		if(texture == NULL)
			fprintf(stderr, "Error texture null", SDL_GetError());
	}

	SDL_FreeSurface(surface);

	level.background = texture;

	return &level;
}
void levelDraw(Level *level, SDL_Renderer *renderer)
{
	SDL_Rect src,dest;

	src.x = 0;
	src.y = 0;
    src.w = 0;
    src.h = 0;
    dest.x = 0;
    dest.y = 0;
    dest.w = 0;
    dest.h = 0;
	
	SDL_RenderCopy(renderer, level->background, NULL, NULL);
}
void levelFree(Level **level)
{

}
