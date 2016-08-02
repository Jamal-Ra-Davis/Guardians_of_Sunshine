#pragma once
#include <SDL2/SDL.h>
#include "../headers/TileMap.h"
#include "../headers/Animation.h"


class MapObject
{
	protected:
		//Tile stuff
		TileMap *tileMap;
		int tileSize;
		double xmap;
		double ymap;

		//Position and vector
		double x, y;
		double dx, dy;

		//dimensions
		int width;
		int height;

		//collision box
		int cwidth;
		int cheight;

		//collision
		int currRow;
		int currCol;
		double xdest;
		double ydest;
		double xtemp;
		double ytemp;
		bool topLeft;
		bool topRight;
		bool bottomLeft;
		bool bottomRight;

		//Animation
		Animation animation;
		int currentAction;
		int prevAction;
		bool facingRight;
		SDL_Renderer *renderTarget;

		//Movement
		bool left;
		bool right;
		bool up;
		bool down;
		bool jumping;
		bool falling;

		//Movement attributes
		double moveSpeed;
		double maxSpeed;
		double stopSpeed;
		double fallSpeed;
		double maxFallSpeed;
		double jumpStart;
		double stopJumpSpeed;

		void mapObjectDraw();
	public:
		MapObject(TileMap *tm, SDL_Renderer *renderTarget_);
		//~MapObject();

		int getX() {return (int)x;}
		int getY() {return (int)y;}
		int getWidth() {return width;}
		int getHeight() {return height;}
		int getCWidth() {return cwidth;}
		int getCHeight() {return cheight;}

		virtual void setLeft(bool b) {left = b;}
		virtual void setRight(bool b) {right = b;}
		virtual void setUp(bool b) {up = b;}
		virtual void setDown(bool b) {down = b;}
		virtual void setJumping(bool b) {jumping = b;}
		
		bool getJumping() {return jumping;}
		bool getFalling() {return falling;}

		void setPosition(double x_, double y_);
		void setVector(double dx_, double dy_);
		void setMapPosition();

		SDL_Rect getRectangle();
		bool intersects(MapObject *o);

		void calculateCorners(double x_, double y_);
		void checkTileMapCollision();

		//void setAnimation(SDL_Texture* animationTexture, int action, SDL_Rect *anm_rects, int frameCount, int delay, bool playsOnce=false, int loopNum=-1);

		bool notOnScreen();
		SDL_Rect getCollisionRect();
		void drawRect(SDL_Rect rect);
		
};
