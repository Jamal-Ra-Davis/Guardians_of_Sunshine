#include "../headers/Explosion.h"
#include "../headers/SDL_Helpers.h"

Explosion::Explosion(SDL_Renderer *renderTarget_, int x_, int y_)
{
	x = x_;
	y = y_;

	renderTarget = renderTarget_;
	
	remove = false;

	width = 30;
	height = 30;

	animationTexture = NULL;
	animationTexture = LoadTexture("./Resources/Sprites/Enemies/explosion.bmp", renderTarget);

	numSprites = 4;
	sprite_rects = new SDL_Rect[numSprites];
	for (int i=0; i<numSprites; i++)
    {
        sprite_rects[i].x = i*width;
        sprite_rects[i].y = 0;
        sprite_rects[i].w = width;
        sprite_rects[i].h = height;
    }
	animation.setFrames(animationTexture, sprite_rects, numSprites);
	animation.setDelay(70);	
}
Explosion::~Explosion()
{
	delete [] sprite_rects;
    sprite_rects = NULL;

    SDL_DestroyTexture(animationTexture);
    animationTexture = NULL;
}
void Explosion::update()
{
	animation.update();
	if (animation.hasPlayedOnce())
	{
		remove = true;
	}
}
void Explosion::setMapPosition(int x_, int y_)
{
	xmap = x_;
	ymap = y_;
}
void Explosion::draw()
{
	SDL_Rect posRect = {(int)(x + xmap - width/2), (int)(y + ymap - height/2), width, height};
    SDL_Rect cropRect = animation.getImageRect();
    SDL_RenderCopy(renderTarget, animation.getFrameTexture(), &cropRect, &posRect);
}
