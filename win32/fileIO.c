#include "fileIO.h"

int counter = 0;
int positionFile = 0;

void saveContents(int i, char *filename)
{
	FILE *fp;

	fp = fopen(filename, "a");

	if(fp == NULL)
		printf("Error on file opening");

	fprintf(fp, "%i\n", i);

	fclose(fp);
}

void saveSPointer(Sprite *s, char *filename)
{
	FILE *fp;

	fp = fopen(filename, "a");

	if(fp == NULL)
		printf("Error on file opening");

	fprintf(fp, "%p\n", s);

	fclose(fp);
}

void saveEPointer(void *e, char *filename)
{
	FILE *fp;

	fp = fopen(filename, "a");

	if(fp == NULL)
		printf("Error on file opening");

	fprintf(fp, "%p\n", e);

	fclose(fp);
}

int loadContents(char *filename)
{
	FILE *fp;
	int c = 0;
	int i = 0;
	char singleLine[10];

	fp = fopen(filename, "r");

	if(fp == NULL)
		printf("Error on file opening");

	for(counter = 0; positionFile>=counter; counter++)
	{
		fgets(singleLine, 10, fp);
	}

	sscanf(singleLine, "%i", &c);

	positionFile++;
	fclose(fp);

	if(c == 999999)
	{
		counter = 0;
		positionFile = 0;
	}

	return(c);
}

Sprite *fileloadSprite(char *filename)
{
	FILE *fp;
	Sprite *c = NULL;
	int i = 0;
	char singleLine[10];

	fp = fopen(filename, "r");

	if(fp == NULL)
		printf("Error on file opening");

	for(counter = 0; positionFile>=counter; counter++)
	{
		fgets(singleLine, 10, fp);
	}

	sscanf(singleLine, "%p", &c);

	positionFile++;
	fclose(fp);

	return(c);
}
void *loadPointer(char *filename)
{
	FILE *fp;
	void *c = NULL;
	int i = 0;
	char singleLine[10];

	fp = fopen(filename, "r");

	if(fp == NULL)
		printf("Error on file opening");

	for(counter = 0; positionFile>=counter; counter++)
	{
		fgets(singleLine, 10, fp);
	}

	sscanf(singleLine, "%p", &c);

	positionFile++;
	fclose(fp);

	return(c);
}
char *loadHelp(char *filename)
{
	FILE *fp;
	char help[500];
	char *a;

	fp = fopen(filename, "r");

	while(!feof(fp))
	{
		fgets(help, 500, fp);
	}

	a = help;

	fclose(fp);
	
	return a;
}