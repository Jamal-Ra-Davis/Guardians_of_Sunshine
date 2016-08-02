#pragma once
#include <SDL2/SDL.h>

class Tile
{
	private:
		SDL_Texture *image;
		SDL_Rect cropRect;
		//SDL_Rect posRect;
		int type;
		int *heightMap;
	public:
		enum TileTypes{NORMAL, BLOCKED, HEIGHT_MAP, NUM_TILE_TYPES};
		Tile(SDL_Texture *image_, int cropX, int cropY, int cropWidth, int cropHeight, int type_);
		~Tile();
		SDL_Texture *getImage() {return image;}
		SDL_Rect getCropRect() {return cropRect;}
		int getType() {return type;}
		void setType(int type_) {type = type_;}
		void setHeightMap(int *heights);
		bool checkCollision(int x, int y);
};
