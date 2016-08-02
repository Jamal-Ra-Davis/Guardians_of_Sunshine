#pragma once

#include <SDL2/SDL.h>
#include "../headers/MapObject.h"
#include "../headers/InputBuffer.h"
class ComboMove : public MapObject
{
	private:
		bool visible;
		bool active;
		bool active_trigger;
		bool spin;
		double combo_time;//~8 seconds 
		double start_time;

		bool combo_pass;
		bool combo_fail;
	
		InputBuffer inputBuffer;	

		//Animations
        int numAnimations;
        SDL_Texture *animationTexture;
        SDL_Rect **sprite_rects;
        int *frameNumbers;		
		
		int action_idx;

	public:
		enum ComboMove_Actions {IDLE, RIGHT, LEFT, DOWN, UP, JUMP, SPIN, SAD_IDLE, SAD_RIGHT, SAD_LEFT, SAD_DOWN, SAD_UP, SAD_JUMP, NUM_COMBO_MOVES};
		ComboMove(TileMap *tm, SDL_Renderer *renderTarget_);
		~ComboMove();
		
		bool getActive() {return active;}
		void setActive(bool b) {active = b;}
		bool getVisible() {return visible;}
		void setVisible(bool b) {visible = b;}
		bool getSpin() {return spin;}
		bool comboPassed() {return combo_pass;}
		bool comboFailed() {return combo_fail;}
		bool foundInputFullMatch() {return inputBuffer.foundFullMatch();}
		bool foundInputMidMatch() {return inputBuffer.foundMidMatch();}
		bool foundInputHalfMatch() {return inputBuffer.foundHalfMatch();}

		void setLeft(bool b); 
        void setRight(bool b); 
        void setUp(bool b); 
        void setDown(bool b); 
        void setJumping(bool b); 
		void setSpin(bool b);
		bool isBusy();
		void reset();

		void onActivate();
		void update();
		void draw();
		
};
