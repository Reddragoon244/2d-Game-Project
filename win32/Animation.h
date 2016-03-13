#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <SDL.h>

typedef struct{

	float deltaTime;

}Animation;

int sprite_animation(int frameMax, int startFrame, int FrameSpeed, float &frameTime);
float time_animation();

#endif