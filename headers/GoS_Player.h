#pragma once
#include <SDL2/SDL.h>
#include <list>
#include "../headers/MapObject.h"
#include "../headers/Enemy.h"
#include "../headers/AudioPlayer.h"
#include "../headers/Bomba.h"



class GoS_Player : public MapObject
{
	private:
		//Player stuff
		int health;
		int maxHealth;
		bool dead;
		bool onGround;
		bool jumpInit;
		bool jumpAction;
		bool hold;
		bool dying;
		bool visible;
		bool start;		

		bool flinching;
		double flinchTimer;

		double danceTimer; 
		//Bomba
		bool bombaStarting;	
		bool bombaHolding;
		bool bombaThrowing;
		int bombaCount;
		int maxBombaCount;
		int bombaDamage;
		Bomba *activeBomba;
		std::list<Bomba*> bombas;

		//Kick attack
		bool kicking;
		int kickDamage;
		int kickRange;
		double kickStartX;
	
		//Animations
		int numAnimations;
		SDL_Texture *animationTexture;
		SDL_Rect **sprite_rects;
		int *frameNumbers;

		//Audio
		AudioPlayer **player_sounds;
	
		void getNextPosition();

	public:
		enum Player_Actions {DANCING, FRONT_FACE, WALKING, IDLE, JUMPING_START, JUMPING_HOLD, JUMPING_LAND, LANDING, KICKING, BOMBA_START, BOMBA_THROW, HIT, DYING};
		enum Player_Sounds {DANCE_SFX, WALK_SFX, JUMP_SFX, KICK_SFX, HIT_SFX, DIE_SFX, BOMBA_START_SFX, BOMBA_FUSE_SFX, BOMBA_THROW_SFX, BOMBA_HIT_SFX, SFX_NUM};

		GoS_Player(TileMap *tm, SDL_Renderer *renderTarget_);
        ~GoS_Player();

		int getHealth() {return health;}
        int getMaxHealth() {return maxHealth;}
		
		int getBombaCount() {return bombaCount;}
		int getMaxBombaCount() {return maxBombaCount;}

		void bombaAction();
		void setBombaStarting() {bombaStarting = true;}
		void setBombaThrowing() {bombaThrowing = true;}
		void setKicking() {if (!isBusy()) kicking = true;}
		bool isDead() {return dead;}
		void setDead(bool b) {dead = b;}
		bool isOnGround() {return onGround;}
		bool getHold() {return hold;}
		void setHold(bool b);// {hold = b;}
		bool getVisible() {return visible;}
		void setVisible(bool b) {visible = b;}
		bool isBusy();	

		void reset(double x_, double y_);
		void hit(int damage);
        void update();
        void draw();

		void checkAttack(std::list<Enemy*> enemies);
};
