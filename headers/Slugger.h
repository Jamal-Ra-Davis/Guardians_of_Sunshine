#pragma once
#include <SDL2/SDL.h>

#include "../headers/Enemy.h"

class Slugger : public Enemy
{
	private:
		SDL_Texture *animationTexture;
		SDL_Rect *sprite_rects;
		int numSprites;

		void getNextPosition();
	public:
		Slugger(TileMap *tm, SDL_Renderer *renderTarget_);
		~Slugger();

		void update();
		void draw();
		
};
