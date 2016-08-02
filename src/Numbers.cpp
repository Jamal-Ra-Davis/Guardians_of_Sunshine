#include "../headers/Numbers.h"
#include "../headers/SDL_Helpers.h"
#include <math.h>

Numbers::Numbers(SDL_Renderer *renderTarget_, int digits_, int spacing_, int x_, int y_, bool visible_)
{
	renderTarget = renderTarget_;
	digits = digits_;
	spacing = spacing_;
	x = x_;
	y = y_;
	visible = visible_;
	
	numberTexture = NULL;
	numberTexture = LoadTexture("./Resources/Sprites/GoS_Numbers.bmp", renderTarget);
	xOffset = 0;
    yOffset = 0;	
}
Numbers::~Numbers()
{
	SDL_DestroyTexture(numberTexture);
	numberTexture = NULL;
}
void Numbers::draw(int number)
{
	if (!visible)
		return;

	if (digits <= 0 || digits > MAX_DIGITS)
		return;
	SDL_Rect numRect = {x+xOffset, y+yOffset, NUM_WIDTH, NUM_HEIGHT};
	int mod = (int)pow(10, digits);
	number = number % mod;
	for (int i=0; i<digits; i++)
	{
		mod /= 10;
		int dig = number/mod;
		number = number % mod;
		//printf("%d-", dig);
		SDL_Rect cropRect = {dig*NUM_WIDTH, 0, NUM_WIDTH, NUM_HEIGHT};
		SDL_RenderCopy(renderTarget, numberTexture, &cropRect, &numRect);
		numRect.x += NUM_WIDTH + spacing;	
	}
	//printf("\n");
}
