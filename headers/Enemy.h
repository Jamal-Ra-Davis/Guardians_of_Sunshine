#pragma once

#include "../headers/MapObject.h"
#include "../headers/Bomba.h"
#include "../headers/TriggerBox.h"

class Enemy : public MapObject
{
	protected:
		int health;
		int maxHealth;
		bool dead;
		int damage;
		int score;


		bool flinching;
		double flinchTimer;

		TriggerBox *triggerBox;

	public:
		Enemy(TileMap *tm, SDL_Renderer *renderTarget_) : MapObject(tm, renderTarget_) {}
		bool isDead() {return dead;}
		void setDead(bool b) {dead = b;}
		int getDamage() {return damage;}
		virtual void hit(int damageTaken);
		
		int getScore() {return score;}
		void setScore(int s) {score = s;}

		int getHealth() {return health;}
		int getMaxHealth() {return maxHealth;}

		void setTriggerBox(TriggerBox *tb) {triggerBox = tb;}
        TriggerBox* getTriggerBox() {return triggerBox;}

		virtual bool isReady() {};
		virtual void checkBombaCollision(Bomba *bomba) {};
		virtual void getPoofPosition(double *x_, double *y_);
		virtual void update() {};
		virtual void draw() {};
};
