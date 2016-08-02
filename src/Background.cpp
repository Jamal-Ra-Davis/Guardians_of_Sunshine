#include <math.h>

#include "../headers/Background.h"
#include "../headers/SDL_Helpers.h"
#include "../headers/GamePanel.h"

Background::Background(std::string s, double ms, SDL_Renderer *renderTarget_)
{
	renderTarget = renderTarget_;
	bgImage = NULL;
	bgImage = LoadTexture(s.c_str(), renderTarget);
	SDL_QueryTexture(bgImage, NULL, NULL, &textureWidth, &textureHeight);
	moveScale = ms;
}
Background::~Background()
{
	if (bgImage != NULL)
		SDL_DestroyTexture(bgImage);
	bgImage = NULL;
}
void Background::setPosition(double x_, double y_)
{
	//x = (x_ * moveScale) % GamePanel::WIDTH;
	//y = (y_ * moveScale) % GamePanel::HEIGHT;
	x = fmod((x_ * moveScale),  GamePanel::WIDTH);
	y = fmod((y_ * moveScale),  GamePanel::HEIGHT);
	//x = x_;
	//y = y_;
}
void Background::setVector(double dx_, double dy_)
{
	dx = dx_;
	dy = dy_;
}
void Background::update()
{

	x += dx*moveScale;
	x += dy*moveScale;
	x = fmod(x,  GamePanel::WIDTH);
    y = fmod(y,  GamePanel::HEIGHT);

//	x += dx;
//	y += dy;
}
void Background::draw()
{
	SDL_Rect posRect;
	posRect.x = (int)x;
	posRect.y = (int)y;
	posRect.w = textureWidth;
	posRect.h = textureHeight;

	SDL_RenderCopy(renderTarget, bgImage, NULL, &posRect);
	if (x < 0)
	{
		posRect.x = (int)x + GamePanel::WIDTH;
		SDL_RenderCopy(renderTarget, bgImage, NULL, &posRect);
	}
	else if (x > 0)
	{
		posRect.x = (int)x - GamePanel::WIDTH;
		SDL_RenderCopy(renderTarget, bgImage, NULL, &posRect);
	}
}
