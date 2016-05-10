#include "font.h"

Font *fontList = NULL;
Font FontList[MAX_FONT];
Font *Mfont = NULL;
int NumFont;

void InitFontSystem()
{
	fontList = (Font *)malloc(sizeof(Font)*MAX_FONT);
	memset(fontList, 0,sizeof(Font)*MAX_FONT);

	if(TTF_Init() < 0)
		printf("Error: %s", TTF_GetError());

	atexit(CloseFontSystem);
}
void CloseFontSystem()
{	
   int i;

   for(i = 0;i < MAX_FONT;i++)
   {
     /*it shouldn't matter if the music is already freed, 
     freeMusic checks for that*/
      freeFont(&FontList[i]);
   }
}

Font *loadFont(char *filename, int size, char *TEXT, SDL_Renderer *renderer)
{
	int i;

	if(NumFont + 1 >= MAX_FONT)
	{
		fprintf(stderr, "Maximum Font Effects Reached. \n");
	}

	NumFont++;
	for(i=0; i<=NumFont; i++)
	{
		if(!FontList[i].inuse)break;
	}

	FontList[i].inuse++;

	FontList[i].font =  TTF_OpenFont(filename, size);/*Font Load from File*/
	
	SDL_Colour color = {144, 77, 255, 255};
	SDL_Surface *textSurface = TTF_RenderText_Solid(FontList[i].font, TEXT, color);
	FontList[i].text = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_FreeSurface(textSurface);
	textSurface = NULL;

	SDL_QueryTexture(FontList[i].text, NULL, NULL, &FontList[i].fontSize.w, &FontList[i].fontSize.h);

	strcpy(FontList[i].filename, filename);

	return &FontList[i];
	
}

void textChange(Font *f, char *TEXT, SDL_Renderer *renderer)
{
	SDL_Colour color = {144, 77, 255, 255};
	SDL_Surface *textSurface = TTF_RenderText_Solid(f->font, TEXT, color);
	f->text = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_FreeSurface(textSurface);
	textSurface = NULL;

	SDL_QueryTexture(f->text, NULL, NULL, &f->fontSize.w, &f->fontSize.h);
}

void freeFont(Font *f)
{
	/*Need to clear the inuse*/
	f->inuse--;
	if(f->inuse == 0)
	{
		/*Nullify the filename*/
		strcpy(f->filename, "\0");
		/*Nullify the sound effect*/
		if(f->font != NULL)f->font = NULL;
	}
}

void font_draw(Font_t *FONT, SDL_Renderer *renderer, int drawX, int drawY, SDL_Rect &Camera, float scale)
{

	FONT->fontSize.x = drawX;
	FONT->fontSize.y = drawY;
	FONT->fontSize.w *= scale;
	FONT->fontSize.h *= scale;

	SDL_RenderCopy(renderer, FONT->text, NULL, &FONT->fontSize);
}