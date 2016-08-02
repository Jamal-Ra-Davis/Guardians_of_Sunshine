#pragma once
#include <SDL2/SDL.h>
#include "../headers/Animation.h"

class Explosion 
{
	private:
		int x, y;
		int xmap, ymap;
		int width, height;

		SDL_Texture *animationTexture;
		SDL_Rect *sprite_rects;		
		int numSprites;
		bool remove;
		
		SDL_Renderer *renderTarget;
		Animation animation;

	public:
		Explosion(SDL_Renderer *renderTarget_, int x_, int y_);
		~Explosion();
		void update();
		void setMapPosition(int x_, int y_);
		bool shouldRemove() {return remove;}
		void draw();
};
