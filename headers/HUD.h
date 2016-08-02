#pragma once
#include <SDL2/SDL.h>

#include "../headers/Player.h"

class HUD
{
	private:
		Player *player;

		SDL_Texture *hudImage;
		SDL_Rect hudImageRect;

		SDL_Texture *heartImage;		
		SDL_Texture *eHeartImage;
	
		SDL_Renderer *renderTarget;
	public:
		HUD(Player *player_, SDL_Renderer *renderTarget_, int x=0, int y=20);
		~HUD();
		void setHudPosition(int x, int y) {hudImageRect.x = x; hudImageRect.y = y;}
		void draw();
};
