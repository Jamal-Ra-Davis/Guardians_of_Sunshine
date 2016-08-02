#pragma once
#include <SDL2/SDL.h>

#include "../headers/GameState.h"


//enum GameStateNames
//{
//	MENU_STATE, LEVEL1_STATE, NUM_STATES
//};

class GameState;
class GameStateManager
{
	private:
		GameState **gameStates;
		int currentState;
		SDL_Renderer *renderTarget;
		int score;

		void loadState(int state);
		void unloadState(int state);
	public:
		enum GameStateNames
		{
    		MENU_STATE, TITLE_STATE, LEVEL1_STATE, WIN_STATE, NUM_STATES
		};

		GameStateManager(SDL_Renderer *renderTarget_);
		~GameStateManager();
		int getScore() {return score;}
		void setScore(int s) {score = s;}

		void setState(int state);
		void update();
		void draw();

		void keyPressed(int k);
		void keyReleased(int k);

};
