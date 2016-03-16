#ifndef __ITEMS_H__
#define __ITEMS_H__
#include "entity.h"
#include <string>

int doorThink(Entity_t *self, int endPosition);
int leverThink(Entity_t *self);
void boxThink(Entity_t *self, Entity_t *player, SDL_Rect t, SDL_Rect r, SDL_Rect &Box);
int buttonThink(Entity_t *self);

#endif