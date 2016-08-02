#pragma once
#include <SDL2/SDL.h>
#include "../headers/MapObject.h"
#include "../headers/AudioPlayer.h"

class Coin : public MapObject
{
	private:
		int value;
		bool collected;
		bool should_remove;

		double collectTimer;
		int disappearTime;
		//Animations
        //int numAnimations;
        SDL_Texture *animationTexture;
        SDL_Rect **sprite_rects;
        int *frameNumbers;
		AudioPlayer *coin_sound;		

	public:
		enum Coin_States {DEFAULT, COLLECTED, DISAPPEARING, NUM_ANIMATIONS};
		Coin(TileMap *tm, SDL_Renderer *renderTarget_, int value_);
		~Coin();

		bool isCollected() {return collected;}
		bool shouldRemove() {return should_remove;}	
		int getValue() {return value;}

		void collect();
		void update();
		void draw();
};
