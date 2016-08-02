#include "../headers/Bomba.h"
#include "../headers/SDL_Helpers.h"

Bomba::Bomba(TileMap *tm, SDL_Renderer *renderTarget_, bool right, int damage_) : MapObject(tm, renderTarget_)
{
	active = false;
	remove = false;
	damage = damage_;
	facingRight = right;

	width = 13;
    height = 23;
    cwidth = 13;
    cheight = 23;

    //load sprites
    animationTexture = LoadTexture("./Resources/Sprites/Player/GoS_Bomba.bmp", renderTarget);
    //if (animationTexture)
    //    printf("DEBUG: Loaded fireball textures\n");

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
    animation.setDelay(50);

}
Bomba::~Bomba()
{
	if (sprite_rects)
		delete [] sprite_rects;
	sprite_rects = NULL;
	
	SDL_DestroyTexture(animationTexture);
    animationTexture = NULL;
}

void Bomba::update()
{
	animation.update();
	if (!active)
		return;

	xdest = x + dx;
    ydest = y + dy;

	if (tileMap->checkTileCollision(int(xdest), (int)(ydest)))
	{
		//Collided with tilemap
		remove = true;
	}
	if (xdest < 0 - cwidth/2)
	{
		remove = true;
	}
	x = xdest;
	y = ydest;
		
}
void Bomba::draw()
{
	setMapPosition();
    mapObjectDraw();
}


