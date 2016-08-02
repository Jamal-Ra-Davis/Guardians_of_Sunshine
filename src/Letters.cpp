#include "../headers/Letters.h"
#include "../headers/SDL_Helpers.h"
#include <math.h>

Letters::Letters(SDL_Renderer *renderTarget_, std::string word_, int spacing_, int x_, int y_, bool visible_)
{
	renderTarget = renderTarget_;
	word = word_;
	spacing = spacing_;
	x = x_;
	y = y_;
	visible = visible_;

	letterTexture = NULL;
	letterTexture = LoadTexture("./Resources/Sprites/GoS_Letters.bmp", renderTarget);
	xOffset = 0;
	yOffset = 0;	
}
Letters::~Letters()
{
	SDL_DestroyTexture(letterTexture);
	letterTexture = NULL;
}
void Letters::draw()
{
	if (!visible)
		return;

	SDL_Rect letterRect = {x+xOffset, y+yOffset, LETTER_WIDTH, LETTER_HEIGHT};
	for (int i=0; i<word.length(); i++)
	{
		char c = toupper(word[i]);
		int idx = c - 65;	

		SDL_Rect cropRect = {idx*LETTER_WIDTH, 0, LETTER_WIDTH, LETTER_HEIGHT};
		SDL_RenderCopy(renderTarget, letterTexture, &cropRect, &letterRect);
		letterRect.x += LETTER_WIDTH + spacing;	
	}
	//printf("\n");
}
