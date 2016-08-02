#pragma once
#include <SDL2/SDL.h>
#include <string>

#include "../headers/Tile.h"

class TileMap
{
	private:
		SDL_Renderer *renderTarget;

		//Posistion
		double x, y;

		//Bounds
		int xmin, ymin;
		int xmax, ymax;
		double tween;

		//Map
		int **map;
		int tileSize;
		int numRows, numCols;
		int width, height;

		//Tileset
		SDL_Texture *tileset;
		int numTilesAcross;
		int numTilesVert;
		Tile ***tiles;

		//drawing
		int rowOffset, colOffset;
		int numRowsToDraw, numColsToDraw;

		bool fix_bounds;
		void fixBounds();
		
	public:
		TileMap(int tileSize_, SDL_Renderer *renderTarget_);
		~TileMap();
		void loadTiles(std::string tilesheet_name, std::string tiletype_file);
		void loadMap(std::string s);

		int getNumRows() {return numRows;}
        int getNumCols() {return numCols;}

		int getTileSize() {return tileSize;}
		double getX() {return x;}
		double getY() {return y;}
		int getWidth() {return width;}
		int getHeight() {return height;}

		int getType(int row, int col);
		Tile* getTile(int row, int col);
		Tile* getTileFromPos(int x, int y);
		bool checkTileCollision(int x, int y);
		void setPosition(double x, double y);
		void setTween(double t) {tween = t;}		
		void setFixBounds(bool b) {fix_bounds = b;}	
	
		void draw();	
};
