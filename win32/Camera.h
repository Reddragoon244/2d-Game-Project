#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <SDL.h>
#include "vector.h"
#include "level.h"

static SDL_Rect Camera = {0, 0, 1080, 720};

SDL_Rect cameraGetActiveCamera();

void camera_set_position(Vector2d pos);
void camera_set_size(Vector2d size);
void camera_live_set(SDL_Rect &a);

#endif