#pragma once
#include <SDL2/SDL.h>
#include "../headers/Enemy.h"
#include "../headers/ComboMove.h"
#include "../headers/AudioPlayer.h"

class SleepySam : public Enemy
{
	private:
		bool active;
		bool visible;	
		bool eat_player;
		bool eat_start;	
		bool playedPoof;
		double poof_x, poof_y;
		double animationTimer;
		bool animation_flag;
		bool finished_eating;
		ComboMove *comboMove;

		int numAnimations;
        SDL_Texture *animationTexture;
        SDL_Rect **sprite_rects;
        int *frameNumbers;		

		AudioPlayer **frog_sounds;
		
	public:
		enum Frog_Actions {TEST, IDLE, EATING, POOPING, POOP_FALL};
		enum Frog_Sounds {LICK_SFX, SWALLOW_SFX, POOP_SFX, POOP_FALL_SFX, SFX_NUM};
		SleepySam(TileMap *tm, SDL_Renderer *renderTarget_);
		~SleepySam();

		bool isVisible() {return visible;}
		void setVisible(bool b) {visible = b;}
		void setEatPlayer(bool b) {eat_player = true;}	
		void setComboMove(ComboMove *cm) {comboMove = cm;}
		bool getActive() {return active;}
		void setActive(bool b) {active = b;}	
		bool getPlayedPoof() {return playedPoof;}
		void setPlayedPoof(bool b) {playedPoof = b;}
		bool getFinishedEating() {return finished_eating;}

		void hit(int damageTaken) {};
		void checkBombaCollision(Bomba *bomba) {};
		bool isReady();
		void getPoofPosition(double *x_, double *y_);
		void reset();
		void update();
		void draw();


};
