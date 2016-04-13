#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <SDL.h>

typedef struct{

	float deltaTime;

}Animation;

void sprite_animation(int frameMax, int startFrame, int FrameSpeed, float &frameTime, int &frame);
float time_animation();

#endif