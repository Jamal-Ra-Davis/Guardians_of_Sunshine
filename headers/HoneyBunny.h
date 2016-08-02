#pragma once
#include <SDL2/SDL.h>
#include "../headers/Enemy.h"
#include "../headers/TriggerBox.h"
#include "../headers/Animation.h"
#include "../headers/Bomba.h"
#include "../headers/AudioPlayer.h"

class HoneyBunny : public Enemy
{
	private:
		bool active;

		bool bunny_visible;
		bool slime_visible;
		bool spawned;
	
		bool face_collision;
		SDL_Rect face_rect;
        //TriggerBox *triggerBox;
		bool triggerState;

		double poof_x, poof_y;

        int numAnimations;
        SDL_Texture *animationTexture;
        SDL_Rect **sprite_rects;
        int *frameNumbers;

		int numSlimeAnimations;
        SDL_Texture *slimeAnimationTexture;
        SDL_Rect **slimeSprite_rects;
        int *slimeFrameNumbers;
		
		int slimeWidth;
		int slimeHeight;
		int slimeCWidth;
		int slimeCHeight;
		Animation slimeAnimation;
		int slimeCurrentAction;

		AudioPlayer *bunny_sound;

	public:
		enum Bunny_Actions{TEST, SPAWNING, IDLE, MOVE_ARMS, HIT};
		enum Slime_Action{SLIME_TEST, START, LOOP};

		HoneyBunny(TileMap *tm, SDL_Renderer *renderTarget_);
		~HoneyBunny();

        bool isActive() {return active;}
        void setActive(bool b) {active = b;}
		bool isBunnyVisible() {return bunny_visible;}
		void setBunnyVisible(bool b) {bunny_visible = b;}
		bool isSlimeVisible() {return slime_visible;}
        void setSlimeVisible(bool b) {slime_visible = b;}

        //void setTriggerBox(TriggerBox *tb) {triggerBox = tb;}
        //TriggerBox* getTriggerBox() {return triggerBox;}

		void setAnimation(int action, int delay, bool playsOnce=false, int loopNum=-1);
		void onTrigger();
        void hit(int damageTaken);
		bool isReady();
		void checkBombaCollision(Bomba *bomba);
		void getPoofPosition(double *x_, double *y_);
        void update();
        void draw();
};
