#ifndef __PHYSICS_H__
#define __PHYSICS_H
#include <SDL.h>
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "entity.h"

void moveRight(SDL_Rect &r, SDL_Rect &t,  float deltaTime, Entity_t *a, int moveSpeed);
void moveLeft(SDL_Rect &r, SDL_Rect &t, float deltaTime, Entity_t *a, int moveSpeed);
void Jump(SDL_Rect &r, SDL_Rect &t,  float deltaTime, Entity_t *a, int moveSpeed, int jumpMax);
void Fall(SDL_Rect &r, SDL_Rect &t,  float deltaTime, Entity_t *a, int moveSpeed);

#endif