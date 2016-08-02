#pragma once
#include <SDL2/SDL.h>
#include "../headers/MapObject.h"

class TriggerBox
{
	private:
		SDL_Rect bounding_box;
		bool tripped;
		bool visible;
	public:
		TriggerBox(int x, int y, int w, int h, bool v=false);
		int getPosX() {return bounding_box.x;}
		int getPosY() {return bounding_box.y;}
		int getWidth() {return bounding_box.w;}
		int getHeight() {return bounding_box.h;}

		bool intersectsObject(MapObject *o);
		bool wasTripped() {return tripped;}
		void reset() {tripped = false;}
		bool isVisible() {return visible;}
		void makeVisible(bool v) {visible = v;}
		void draw(TileMap *tm, SDL_Renderer *renderTarget);
};
