#include "sound.h"

Music *musicList = NULL;
SoundEffect *soundList = NULL;

Music MusicList[MAX_MUSIC];
SoundEffect SoundList[MAX_SOUND];

Music *Mmusic = NULL;
SoundEffect *Msoundeffect = NULL;

int NumMusic;
int NumSound;

void InitSoundSystem()
{
	musicList = (Music *)malloc(sizeof(Music)*MAX_MUSIC);
	memset(MusicList, 0,sizeof(Music)*MAX_MUSIC);

	soundList = (SoundEffect *)malloc(sizeof(SoundEffect)*MAX_SOUND);
	memset(SoundList, 0,sizeof(SoundEffect)*MAX_SOUND);

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		printf("Error %s", Mix_GetError());

	atexit(CloseSoundSystem);
}

SoundEffect *loadSound(char *filename)
{
	int i;

	if(NumSound + 1 >= MAX_SOUND)
	{
		fprintf(stderr, "Maximum Sound Effects Reached. \n");
	}

	NumSound++;
	for(i=0; i<=NumSound; i++)
	{
		if(!SoundList[i].inuse)break;
	}

	SoundList[i].inuse = 1;
	SoundList[i].repeat = 0;
	strcpy(SoundList[i].filename, filename);
	SoundList[i].seffect = Mix_LoadWAV(filename); /*.wav, .aiff, .og, .voc*/

	return &SoundList[i];
}

Music *loadMusic(char *filename)
{
	int i;

	if(NumSound + 1 >= MAX_SOUND)
	{
		fprintf(stderr, "Maximum Sound Effects Reached. \n");
	}

	NumMusic++;
	for(i=0; i<=NumMusic; i++)
	{
		if(!MusicList[i].inuse)break;
	}

	MusicList[i].inuse = 1;
	strcpy(MusicList[i].filename, filename);
	MusicList[i].music = Mix_LoadMUS(filename);

	return &MusicList[i];
}

void freeSound(SoundEffect *s)
{
	/*Need to clear the inuse*/
	s->inuse--;
	if(s->inuse == 0)
	{
		/*Nullify the filename*/
		strcpy(s->filename, "\0");
		/*Nullify the sound effect*/
		if(s->seffect != NULL)s->seffect = NULL;
	}
}

void freeMusic(Music *m)
{
	/*Need to clear the inuse*/
	m->inuse--;
	if(m->inuse == 0)
	{
		/*Nullify the filename*/
		strcpy(m->filename, "\0");
		/*Nullify the music*/
		if(m->music != NULL)m->music = NULL;
	}
}

void CloseSoundSystem()
{
   int i;
   int j;

   for(i = 0;i < MAX_MUSIC;i++)
   {
     /*it shouldn't matter if the music is already freed, 
     freeMusic checks for that*/
      freeMusic(&MusicList[i]);
   }
   

   for(j = 0;j < MAX_SOUND;j++)
   {
     /*it shouldn't matter if the sound is already freed, 
     freeSound checks for that*/
      freeSound(&SoundList[i]);
   }
}