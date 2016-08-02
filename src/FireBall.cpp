#include "../headers/FireBall.h"
#include "../headers/SDL_Helpers.h"

FireBall::FireBall(TileMap *tm, SDL_Renderer *renderTarget_, bool right) : MapObject(tm, renderTarget_)
{
	hit = false;
	remove = false;

	facingRight = right;

	moveSpeed = 3.8;
	if (right)
		dx = moveSpeed;
	else
		dx = -moveSpeed;

	width = 30;
	height = 30;
	cwidth = 14;
	cheight = 14;


	//load sprites
	animationTexture = LoadTexture("./Resources/Sprites/Player/fireball.bmp", renderTarget);
	//if (animationTexture)
    //    printf("DEBUG: Loaded fireball textures\n");

	numSprites = 4;
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


    numHitSprites = 3;
    hitSprite_rects = NULL;
    hitSprite_rects = new SDL_Rect[numHitSprites];

	for (int i=0; i<numHitSprites; i++)
    {
        //hitSprite_rects[i] = {i*width, height, width, height};
		hitSprite_rects[i].x = i*width;
        hitSprite_rects[i].y = height;
        hitSprite_rects[i].w = width;
        hitSprite_rects[i].h = height;
    }	

	animation.setFrames(animationTexture, sprite_rects, numSprites);
    animation.setDelay(70);
}
FireBall::~FireBall()
{
	delete [] sprite_rects;
	sprite_rects = NULL;

	delete [] hitSprite_rects;
	hitSprite_rects = NULL;

	SDL_DestroyTexture(animationTexture);
    animationTexture = NULL;
}

void FireBall::setHit()
{
	if (hit)
		return;
	hit = true;
	animation.setFrames(animationTexture, hitSprite_rects, numHitSprites);
	animation.setDelay(70);
	dx = 0;
}
void FireBall::update()
{
	checkTileMapCollision();
	setPosition(xtemp, ytemp);
	animation.update();

	if (dx == 0 && !hit)
	{
		setHit();
	}

	if (hit && animation.hasPlayedOnce())
	{
		remove = true;
	}
}	
void FireBall::draw()
{
	setMapPosition();
	mapObjectDraw();
/*	
	if (facingRight)
    {
        SDL_Rect posRect = {(int)(x + xmap - width/2), (int)(y + ymap - height/2), width, height};
        SDL_Rect cropRect = animation.getImageRect();
        SDL_RenderCopyEx(renderTarget, animation.getFrameTexture(), &cropRect, &posRect, 0, NULL, SDL_FLIP_NONE);
    }
    else
    {
        SDL_Rect posRect = {(int)(x + xmap - width/2), (int)(y + ymap - height/2), width, height};
        SDL_Rect cropRect = animation.getImageRect();
           SDL_RenderCopyEx(renderTarget, animation.getFrameTexture(), &cropRect, &posRect, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
*/
}


