#ifndef __ITEMS_H__
#define __ITEMS_H__
#include "entity.h"
#include <string.h>

void doorThink(Entity_S *self);
void leverThink(Entity_S *self);
void OpenDoor(Entity_S *self, Entity_S *other);
void boxThink(Entity_S *self, Entity_S *player, SDL_Rect t, SDL_Rect r, SDL_Rect &Box);
int buttonThink(Entity_S *self);

#endif