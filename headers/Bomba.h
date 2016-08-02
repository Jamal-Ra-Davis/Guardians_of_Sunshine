#pragma once

#include <SDL2/SDL.h>
#include "../headers/MapObject.h"

class Bomba : public MapObject
{
	private:
		bool active;
		bool remove;
		int damage;
		
		SDL_Texture *animationTexture;
        SDL_Rect *sprite_rects;
        int numSprites;	
	public:
		Bomba(TileMap *tm, SDL_Renderer *renderTarget_, bool right, int damage_=10);
		~Bomba();

		bool isActive() {return active;}
		void setActive(bool b) {active = b;}
		int getDamage() {return damage;}
        bool shouldRemove() {return remove;}
		void setRemove(bool b) {remove = b;}
        void update();
        void draw();
};
