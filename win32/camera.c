#include "Camera.h"

SDL_Rect camera_get_active_camera()
{
    return Camera;
}

void camera_set_position(Vector2d pos)
{
    Camera.x = pos.x;
    Camera.y = pos.y;
}

void camera_set_size(Vector2d size)
{
    Camera.w = size.x;
    Camera.h = size.y;
}

void camera_live_set(SDL_Rect &a)
{
	if(a.x < 0)
		a.x = 0;
	if(a.y < 0)
		a.y = 0;
}
