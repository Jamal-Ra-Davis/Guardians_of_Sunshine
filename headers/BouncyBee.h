#pragma once
#include <SDL2/SDL.h>
#include "../headers/Enemy.h"
#include "../headers/AudioPlayer.h"

class BouncyBee : public Enemy
{
	private:
		bool active;
		bool at_target;
		double targX, targY;
		bool triggerState;
		//TriggerBox *triggerBox;

		int numAnimations;
        SDL_Texture *animationTexture;
        SDL_Rect **sprite_rects;
        int *frameNumbers;

		AudioPlayer *bee_sound;
	public:
		enum Bee_Actions{IDLE, FLYING};
		BouncyBee(TileMap *tm, SDL_Renderer *renderTarget_);
		~BouncyBee();
		
		bool isActive() {return active;}
		void setActive(bool b) {active = b;}
		bool atTarget() {return at_target;}
		void setAtTarget(bool b) {at_target = b;}
		void setTarget(double tX, double tY) {targX = tX; targY = tY;}
		//void setTriggerBox(TriggerBox *tb) {triggerBox = tb;}
		//TriggerBox* getTriggerBox() {return triggerBox;}
		
		void onTrigger();
		void hit(int damageTaken);
		bool isReady();
		void checkBombaCollision(Bomba *bomba);
		void update();
        void draw();
};
