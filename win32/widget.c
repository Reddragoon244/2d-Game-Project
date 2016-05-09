#include "widget.h"

Widget *widgetList = NULL;
Widget WidgetList[MAX_WIDGET];
Widget *Mwidget = NULL;
int NumWidget;

void WidgetSystemInit()
{
	widgetList = (Widget_t *)malloc(sizeof(Widget_t)*MAX_WIDGET);
	memset(WidgetList, 0,sizeof(Widget_t)*MAX_WIDGET);
	atexit(CloseWidgetSystem);
}

void CloseWidgetSystem()
{
   int i;
   for(i = 0;i < MAX_WIDGET;i++)
   {
     /*it shouldn't matter if the entity is already freed, 
     entity_free checks for that*/
      freewidget(&WidgetList[i]);
   }
}

void widget_draw(Widget_t *w, int frame, SDL_Renderer *renderer, int drawX, int drawY, SDL_Rect &Camera, float scaleW, float scaleH)
{
	SDL_Rect src,dest;

	src.x = (frame%w->sprite->fpl) * w->sprite->frameW;//frame%sprite->fpl * sprite->frameW;
	src.y = (frame/w->sprite->fpl) * w->sprite->frameH;//frame/sprite->fpl * sprite->frameH;
    src.w = w->sprite->frameW;
    src.h = w->sprite->frameH;
    dest.x = drawX;
	dest.y = drawY;
    dest.w = w->sprite->frameW*scaleW;
    dest.h = w->sprite->frameH*scaleH;

	w->box.x = drawX;
	w->box.y = drawY;

	SDL_RenderCopy(renderer, w->sprite->image, &src, &dest);
}

int widgetUpdate(Widget_t *w, SDL_Rect m)
{
		if(!w)
	{
		return 0;
	}

		if((w->box.x + w->box.w >= m.x) &&
		(m.x + m.w >= w->box.x) &&
		(w->box.y + w->box.h >= m.y) &&
		(m.y + m.h >= w->box.y))
	{
		printf("%s ", w->sprite->filename);
		return 1;
	}
	else
		return 0;

}

Widget *loadWidget(Sprite *sprite, Font *font, int scale)
{
	int i;

  /*makesure we have the room for a new widget*/
  if(NumWidget + 1 >= MAX_WIDGET)
  {
        fprintf(stderr, "Maximum Widgets Reached.\n");     
  }

  /*if its not already in memory, then load it.*/
  NumWidget++;
  for(i = 0;i <= NumWidget;i++)
  {
    if(!WidgetList[i].inuse)break;
  }

  WidgetList[i].inuse++;

  WidgetList[i].sprite = sprite;

  WidgetList[i].size.w = WidgetList[i].sprite->frameW*scale;
  WidgetList[i].size.h = WidgetList[i].sprite->frameH*scale;
  WidgetList[i].size.x = 0;
  WidgetList[i].size.y = 0;

  WidgetList[i].box.w = WidgetList[i].sprite->frameW*scale;
  WidgetList[i].box.h = WidgetList[i].sprite->frameH*scale;
  WidgetList[i].box.x = 0;
  WidgetList[i].box.y = 0;

  WidgetList[i].font = font;

  WidgetList[i].update = widgetUpdate;
  WidgetList[i].draw = widget_draw;

  return &WidgetList[i];
}

void freewidget(Widget *w)
{
	/*first lets check to see if the widget sprite is still being used.*/
  w->inuse--;
  if(w->inuse == 0)
  {
		strcpy(w->sprite->filename,"\0");
		
		if(w->sprite->image != NULL)SDL_DestroyTexture(w->sprite->image);
			w->sprite->image = NULL;
  }
 /*and then lets make sure we don't leave any potential seg faults 
  lying around*/
}