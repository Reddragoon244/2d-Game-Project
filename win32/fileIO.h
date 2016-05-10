#ifndef __FILEIO_H__
#define __FILEIO_H__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "sprite.h"

void saveContents(int i, char *filename);
void saveSPointer(Sprite *s, char *filename);
void saveEPointer(void *e, char *filename);
int loadContents(char *filename);
Sprite *fileloadSprite(char *filename);
void *loadPointer(char *filename);
char *loadHelp(char *filename);

#endif