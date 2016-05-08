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