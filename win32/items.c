#include "items.h"

void doorThink(Entity_S *self)
{
	if(self->health == 0)
	{
		self->PositionRect.y = 720;
	}
	else
	{

	}
}

void leverThink(Entity_S *self)
{
	if(self->health == 0)
	{
		self->frame = 1;
	}
	else
		self->frame = 0;
}

void OpenDoor(Entity_S *self, Entity_S *other)
{
	if(self->health == 0)
	{
		other->health = 0;
	}
	else
		other->health = 1;
}

void boxThink(Entity_S *self, Entity_S *player, SDL_Rect t, SDL_Rect r, SDL_Rect &Box)
{
	if(self->health == 0)
	{

		if(t.x < r.x)
		{
			Box.x = r.x + 1;
			Box.y = r.y;
		}
		else
			Box.x = r.x - 1;

	}
}

int buttonThink(Entity_S *self)
{
	if(self->health == 0)
	{
		return 1;
	}
	else
		return 0;
}