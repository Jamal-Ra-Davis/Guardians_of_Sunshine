#pragma once

#include <SDL2/SDL.h>
#include "../headers/Animation.h"

class Poof
{
	private:
		int x, y;
        int xmap, ymap;
        int width, height;
		int score;
		bool remove;
		int loopNum;

		//Animations
        int numAnimations;
        SDL_Texture *animationTexture;
        SDL_Rect **sprite_rects;
        int *frameNumbers;
		int currentAction;	

        SDL_Renderer *renderTarget;
        Animation animation;	

	public:
		enum Poof_Actions{START, END};
		Poof(SDL_Renderer *renderTarget_, int x_, int y_, int score_);
		~Poof();
		void setMapPosition(int x_, int y_);
        bool shouldRemove() {return remove;}

		void update();
		void draw();

};
