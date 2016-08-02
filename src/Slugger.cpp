#include "../headers/Slugger.h"
#include "../headers/SDL_Helpers.h"

Slugger::Slugger(TileMap *tm, SDL_Renderer *renderTarget_) : Enemy(tm, renderTarget_)
{
	dx = 0;
	dy = 0;

	moveSpeed = 0.3;
	maxSpeed = 0.3;
	fallSpeed = 0.2;
	maxFallSpeed = 10.0;

	width = 30;
	height = 30;
	cwidth = 20;
	cheight = 20;

	maxHealth = 2;
	health = maxHealth;
	damage = 1;

	dead = false;
	flinching = false;

	//Load in textures and set up animations
	animationTexture = NULL;
	animationTexture = LoadTexture("./Resources/Sprites/Enemies/slugger.bmp", renderTarget);
	numSprites = 3;
    sprite_rects = NULL;
    sprite_rects = new SDL_Rect[numSprites];
	for (int i=0; i<numSprites; i++)
    {
        //sprite_rects[i] = {i*width, 0, width, height};
        sprite_rects[i].x = i*width;
        sprite_rects[i].y = 0;
        sprite_rects[i].w = width;
        sprite_rects[i].h = height;
    }
	animation.setFrames(animationTexture, sprite_rects, numSprites);
    animation.setDelay(300);

	right = true;
	facingRight = right;

}
Slugger::~Slugger()
{
	delete [] sprite_rects;
    sprite_rects = NULL;

    SDL_DestroyTexture(animationTexture);
    animationTexture = NULL;
}
void Slugger::getNextPosition()
{
	//movement
    if (left)
    {
        dx -= moveSpeed;
        if (dx < -maxSpeed)
        {
            dx = -maxSpeed;
        }
    }
    else if (right)
    {
        dx += moveSpeed;
        if (dx > maxSpeed)
        {
            dx = maxSpeed;
        }
    }

	if (falling)
	{
		dy += fallSpeed;
		if (dy > maxFallSpeed)
			dy = maxFallSpeed;
	}
}
void Slugger::update()
{
	getNextPosition();
	checkTileMapCollision();
	setPosition(xtemp, ytemp);

	if (flinching)
	{
		double elapsed = getMs() - flinchTimer;
		if (elapsed > 400)
		{
			flinching = false;
		}
	}

	//if it hits a wall, go other direction
	if (right && dx == 0)
	{
		right = false;
		left = true;
		facingRight = right;
	}
	else if (left && dx == 0)
	{
		right = true;
		left = false;
		facingRight = right;
	}

	animation.update();
}
void Slugger::draw()
{
//	if (notOnScreen())
//		return;

	setMapPosition();
	mapObjectDraw();
}
