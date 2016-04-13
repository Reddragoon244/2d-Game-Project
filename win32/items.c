#include "items.h"

int doorThink(Entity_t *self, int endPosition, int startPosition)
{
	if(self->health == 0)
	{
			return endPosition;
	}
	else
	{
		return startPosition;
	}
}

int leverThink(Entity_t *self)
{
	if(self->health == 0)
	{
		return 1;
	}
	else
		return 0;
}

void boxThink(Entity_t *self, Entity_t *player, SDL_Rect t, SDL_Rect r, SDL_Rect &Box)
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

int buttonThink(Entity_t *self)
{
	if(self->health == 0)
	{
		return 1;
	}
	else
		return 0;
}