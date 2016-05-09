#ifndef __FONT_H__
#define __FONT_H__

#include <SDL_ttf.h>
#include <SDL.h>
#include <SDL_image.h>
#include <cstdlib>
#include <cstdio>
#include <string.h>

const int MAX_FONT = 255;

typedef struct Font_t{

	int inuse;
	char filename[128];
	TTF_Font *font;
	SDL_Texture *text;
	SDL_Rect fontSize;

}Font;


void InitFontSystem();
void CloseFontSystem();

void textChange(Font *f, char *TEXT, SDL_Renderer *renderer);
Font *loadFont(char *filename, int size, char *TEXT, SDL_Renderer *renderer);
void freeFont(Font *f);

void font_draw(Font_t *font, SDL_Renderer *renderer, int drawX, int drawY, SDL_Rect &Camera, float scale);

#endif