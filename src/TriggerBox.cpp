#include "../headers/TriggerBox.h"
#include "../headers/SDL_Helpers.h"

TriggerBox::TriggerBox(int x, int y, int w, int h, bool v)
{
	bounding_box.x = x;
	bounding_box.y = y;
	bounding_box.w = w;
	bounding_box.h = h;
	tripped = false;
	visible = v;
}
bool TriggerBox::intersectsObject(MapObject *o)
{
	if (rectOverlap(bounding_box, o->getRectangle()))
	{
		tripped = true;
		return true;
	}
	else
		return false;
	
}
void TriggerBox::draw(TileMap *tm, SDL_Renderer *renderTarget)
{
	if (!visible)
		return;

	SDL_Rect tempBox = bounding_box;
	tempBox.x -= tempBox.w/2;
	tempBox.y -= tempBox.h/2;

	tempBox.x += tm->getX();
    tempBox.y += tm->getY();
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderTarget, &r, &g, &b, &a);

    SDL_SetRenderDrawColor(renderTarget, 0, 0, 255, 100);
    SDL_RenderFillRect(renderTarget, &tempBox);
    SDL_SetRenderDrawColor(renderTarget, r, g, b, a);
}
