#ifndef __WIDGET_H__
#define __WIDGET_H__

#include <SDL.h>
#include <SDL_image.h>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include "sprite.h"
#include "font.h"
#include "vector.h"

const int MAX_WIDGET = 255;

typedef struct Widget_t{

	SDL_Rect size;
	SDL_Rect box;
	int inuse;
	Sprite_T *sprite;
	Font_t *font;

	void (*draw) (struct Widget_t *self, int frame, SDL_Renderer *renderer, int drawX, int drawY, SDL_Rect &Camera);
	int (*update) (struct Widget_t *self, SDL_Rect m);

}Widget;

void WidgetSystemInit();
void CloseWidgetSystem();
Widget *loadWidget(Sprite *sprite, Font *font);
void freewidget(Widget *w);

#endif