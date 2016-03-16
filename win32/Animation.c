#include "Animation.h"

Animation animation;

	int currentTime = 0;
	int previousTime = 0;
	int frame = 0;
	int slowTimeInit = 500;
	
int sprite_animation(int frameMax, int startFrame, int FrameSpeed, float &frameTime)
{
	if(frameTime >= FrameSpeed)
	{
		if(frame < startFrame)
			frame = startFrame;

		if(frame > frameMax)
			frame = startFrame;
		else
			frame++;

		frameTime = 0;
	}

	return frame;
}

float time_animation()
{
	previousTime = currentTime;
	currentTime = SDL_GetTicks() / 4;
	animation.deltaTime = (currentTime - previousTime);

		return animation.deltaTime;
}