#include "editor.h"

int editorControls()
{
	const Uint8 *num = SDL_GetKeyboardState(NULL);

	if(num[SDL_SCANCODE_1])
		return 1;
	else if(num[SDL_SCANCODE_2])
		return 2;
	else if(num[SDL_SCANCODE_3])
		return 3;
	else if(num[SDL_SCANCODE_4])
		return 4;
	else if(num[SDL_SCANCODE_5])
		return 5;
	else if(num[SDL_SCANCODE_6])
		return 6;
	else if(num[SDL_SCANCODE_7])
		return 9;
	else if(num[SDL_SCANCODE_8])
		return 11;
	else
		return 0;
}