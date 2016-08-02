#pragma once
#include <SDL2/SDL.h>
#include <list>
#include "../headers/MapObject.h"
#include "../headers/FireBall.h"
#include "../headers/Enemy.h"
#include "../headers/AudioPlayer.h"

class Player : public MapObject
{
	private:
		//player stuff
		int health;	
		int maxHealth;
		int fire;
		int maxFire;
		bool dead;
		bool flinching;
		double flinchTimer;
		
		//fireball
		bool firing;
		int fireCost;
		int fireBallDamage;
		std::list<FireBall*> fireBalls;

		//Scratch attack
		bool scratching;
		int scratchDamage;
		int scratchRange;

		//gliding
		bool gliding;

		//Animations
		int numAnimations;
		SDL_Texture *animationTexture;
		SDL_Rect **sprite_rects;
		int *frameNumbers;
	
		AudioPlayer **player_sounds;

		void getNextPosition();	
	public:
		enum Player_Actions {IDLE, WALKING, JUMPING, FALLING, GLIDING, FIREBALL, SCRATCHING};
		enum Player_Sounds {JUMP_SFX, SCRATCH_SFX, SFX_NUM};

		Player(TileMap *tm, SDL_Renderer *renderTarget_);
		~Player();

		int getHealth() {return health;}
		int getMaxHealth() {return maxHealth;}
		int getFire() {return fire;}
		int getMaxFire() {return maxFire;}

		void setFiring() {firing = true;}
		void setScratching() {scratching = true;}
		void setGliding(bool b) {gliding = b;}
		bool isDead() {return dead;}

		void hit(int damage);
		void update();
		void draw();

		void checkAttack(std::list<Enemy*> enemies);
};
