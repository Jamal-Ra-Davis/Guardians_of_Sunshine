#pragma once
#include <SDL2/SDL.h>

#include "../headers/MapObject.h"


class FireBall : public MapObject
{
	private:
		bool hit;
		bool remove;
		
		SDL_Texture *animationTexture;
		SDL_Rect *sprite_rects;
		int numSprites;

		SDL_Rect *hitSprite_rects;
		int numHitSprites;
	

	public:
		FireBall(TileMap *tm, SDL_Renderer *renderTarget_, bool right);
		~FireBall();

		void setHit();
		bool shouldRemove() {return remove;}
		void update();
		void draw();
};
