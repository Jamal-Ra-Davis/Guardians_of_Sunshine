#pragma once
#include <SDL2/SDL.h>
#include "../headers/GoS_Player.h"
#include "../headers/SDL_Helpers.h"
#include "../headers/Numbers.h"

#define NUM_HUD_DIGITS 4

class GoS_HUD
{
	private:
		SDL_Texture *hudBg;
        SDL_Rect hudBgRect;
	
		SDL_Texture *playerIcon;
        SDL_Rect playerRect;

		SDL_Texture *bombaIcon;
        SDL_Rect bombaRect;
	
		SDL_Renderer *renderTarget;
		Numbers *scoreTracker;

	public:
		GoS_HUD(SDL_Renderer *renderTarget_, int x=0, int y=20);
		~GoS_HUD();
		void setHudPosition(int x, int y) {hudBgRect.x = x; hudBgRect.y = y;}
        void draw(int score, int playerLives, int bombaCount);
};
