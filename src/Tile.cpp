#include "../headers/Tile.h"


Tile::Tile(SDL_Texture *image_, int cropX, int cropY, int cropWidth, int cropHeight, int type_)
{
	image = image_;
	cropRect.x = cropX;
	cropRect.y = cropY;
	cropRect.w = cropWidth;
	cropRect.h = cropHeight;
	type = type_;

	heightMap = NULL;
	//heightMap = new int[cropWidth];
}
Tile::~Tile()
{
	if (heightMap)
		delete [] heightMap;
	heightMap = NULL;
}
void Tile::setHeightMap(int *heights)
{
	if (!heightMap)
		heightMap = new int[cropRect.w];
	for (int i=0; i<cropRect.w; i++)
	{
		heightMap[i] = heights[i];
	}
}
bool Tile::checkCollision(int x, int y)
{
	if (type == BLOCKED)
        return true;
	if (type == NORMAL)
		return false;

		
	int x_, y_;
	x_ = x % cropRect.w;
	y_ = y % cropRect.h;

	int mapHeight = heightMap[x_];
	//for (int i=0; i<cropRect.w; i++)
	//	printf("%d ", heightMap[i]);
	//printf("x_: %d, y_: %d, mapHeight: %d\n", x_, y_, mapHeight);
	if (mapHeight == cropRect.h)
		return true;
	else if (mapHeight == 0)
		return false;

	if (mapHeight < 0)
	{
		mapHeight *= -1;
		if (mapHeight > y_)
			return true;
		else
			return false;
	}
	else
	{
		if (mapHeight > cropRect.h - 1 - y_)
			return true;
		else
			return false;
	}
	
}
