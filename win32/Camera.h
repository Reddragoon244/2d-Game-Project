#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <SDL.h>
#include "vector.h"


SDL_Rect cameraGetActiveCamera();
Vector2d cameraPosition();
void cameraSetPosition(Vector2d pos);
void cameraSetSize(Vector2d size);
Vector2d cameraGetSize();


#endif