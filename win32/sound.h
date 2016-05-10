#ifndef __SOUND_H__
#define __SOUND_H__

#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL_mixer.h>

const int MAX_MUSIC = 255;
const int MAX_SOUND = 255;

typedef struct
{
	int inuse;
	Mix_Music *music;
	char filename[128];

}Music;

typedef struct
{	
	int inuse;
	Mix_Chunk *seffect;
	int repeat;
	char filename[128];

}SoundEffect;

void InitSoundSystem();

SoundEffect *loadSound(char *filename);
Music *loadMusic(char *filename);

void freeSound(SoundEffect *s);
void freeMusic(Music *m);

void CloseSoundSystem();


#endif