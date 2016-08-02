#include "../headers/MapObject.h"
#include "../headers/Tile.h"
#include "../headers/GamePanel.h"
#include "../headers/SDL_Helpers.h"

MapObject::MapObject(TileMap *tm, SDL_Renderer *renderTarget_)
{
	left = false;
    right = false;
    up = false;
    down = false;
    jumping = false;
    falling = false;

	tileMap = tm;
	if (tm)
		tileSize = tm->getTileSize();
	renderTarget = renderTarget_;
}
/*
MapObject::~MapObject()
{

}
*/
void MapObject::setPosition(double x_, double y_)
{
	x = x_;
	y = y_;
}
void MapObject::setVector(double dx_, double dy_)
{
	dx = dx_;
	dy = dy_;
}
void MapObject::setMapPosition()
{
	xmap = tileMap->getX();
	ymap = tileMap->getY();
}
SDL_Rect MapObject::getRectangle()
{
	SDL_Rect r;
	r.x = (int)x;
	r.y = (int)y;
	r.w = cwidth;
	r.h = cheight;
	return r;
}
bool MapObject::intersects(MapObject *o)
{
	SDL_Rect r1 = getCollisionRect();
	SDL_Rect r2 = o->getCollisionRect();
	return rectOverlap(r1, r2);
}
void MapObject::calculateCorners(double x_, double y_)
{

	int leftTile = (int)(x_ - cwidth/2)/tileSize;
	int rightTile = (int)(x_ + cwidth/2 - 1)/tileSize;
	int topTile = (int)(y_ - cheight/2)/tileSize;
	int bottomTile = (int)(y_ + cheight/2 - 1)/tileSize;

/*
	int leftTile = ((int)(x - cwidth/2))/tileSize;
	int rightTile = ((int)(x + cwidth/2))/tileSize;
	int topTile = ((int)(y - cheight/2))/tileSize;
	int bottomTile = ((int)(y + cheight/2))/tileSize;

	int currTileX = (int)(x/tileSize);
	int currTileY = (int)(y/tileSize);
*/
	if (topTile < 0 || bottomTile >= tileMap->getNumRows() || leftTile < 0 || rightTile >= tileMap->getNumCols())
	//if (bottomTile < 0 || topTile >= tileMap->getNumRows() || leftTile < 0 || rightTile >= tileMap->getNumCols())
	{
		topLeft = false;
		topRight = false;
		bottomLeft = false;
		bottomRight = false;
		return;
	}

	int tl = tileMap->getType(topTile, leftTile);
	int tr = tileMap->getType(topTile, rightTile);
	int bl = tileMap->getType(bottomTile, leftTile);
	int br = tileMap->getType(bottomTile, rightTile);


	topLeft = (tl == Tile::BLOCKED);
	topRight = (tr == Tile::BLOCKED);
	bottomLeft = (bl == Tile::BLOCKED);
	bottomRight = (br == Tile::BLOCKED);

}
void MapObject::checkTileMapCollision()
{
	currCol = (int)x / tileSize;
	currRow = (int)y / tileSize;

	xdest = x + dx;
	ydest = y + dy;

	xtemp = x;
	ytemp = y;


	calculateCorners(x, ydest);
	if (dy < 0)
	{
		if (topLeft || topRight)
		{
			dy = 0;
			ytemp = currRow * tileSize + cheight/2;
		}
		else
		{
			ytemp += dy;
		}
	}
	if (dy > 0)
	{
		if (bottomLeft || bottomRight)
		{
			dy = 0;
			falling = false;
			ytemp = (currRow+/*1*/2) * tileSize - cheight/2;
		}
		else
		{
			ytemp += dy;
		}
	}


	calculateCorners(xdest, y);
	if (dx < 0)
	{
		if (topLeft || bottomLeft)
		{
			dx = 0;
			xtemp = currCol * tileSize + cwidth/2;
		}
		else
		{
			xtemp += dx;
		}
	}
	if (dx > 0)
	{
		if (topRight || bottomRight)
		{
			dx = 0;
			xtemp = (currCol+1) * tileSize - cwidth/2;
		}
		else
		{
			xtemp += dx;
		}
	}

	if (!falling)
	{
		calculateCorners(x, ydest + 1);
		if (!bottomLeft && !bottomRight)
		{
			falling = true;
		}
	}
}
/*
void MapObject::setAnimation(SDL_Texture* animationTexture, int action, SDL_Rect *anm_rects, int frameCount, int delay, bool playsOnce, int loopNum)
{
	currentAction = action;
    animation.setFrames(animationTexture, anm_rects, frameCount, playsOnce, loopNum);
    animation.setDelay(delay);
    width = sprite_rects[currentAction][animation.getFrame()].w;
}
*/
bool MapObject::notOnScreen()
{
	return( x + xmap + width < 0 || x + xmap - width > GamePanel::WIDTH || y + ymap + height < 0 || y + ymap - height > GamePanel::HEIGHT);
}
void MapObject::mapObjectDraw()
{
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
}
SDL_Rect MapObject::getCollisionRect()
{
	SDL_Rect cRect;
	cRect.x = (int)(x - cwidth/2);
	cRect.y = (int)(y - cheight/2);
	cRect.w = cwidth;
	cRect.h = cheight;
	return cRect;
}
void MapObject::drawRect(SDL_Rect rect)
{
	rect.x += xmap;
	rect.y += ymap;
	Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderTarget, &r, &g, &b, &a);

    SDL_SetRenderDrawColor(renderTarget, 255, 0, 0, 100);
    SDL_RenderFillRect(renderTarget, &rect);
    SDL_SetRenderDrawColor(renderTarget, r, g, b, a);
}
