#ifndef __FILEIO_H__
#define __FILEIO_H__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

void saveContents(int i, char *filename);
int loadContents(char *filename);
char *loadHelp(char *filename);

#endif