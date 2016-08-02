#pragma once

#include <SDL2/SDL.h>
#include <string>
#include "../headers/GameStateManager.h"

class GamePanel
{
	private:
		bool isRunning;
		int FPS;
		//int frameTime;
		int prevTime;
    	int currTime;
    	float deltaTime;

		SDL_Window *window;
		std::string window_name;
    	SDL_Renderer *renderTarget;
		GameStateManager *gsm;

		const Uint8 *keyState;
		SDL_Event event;

	public:
		static bool *isRunningControl;
		static const int WIDTH = 206;//214;//320
		static const int HEIGHT = 116;//141;//240
		GamePanel();
		~GamePanel();

		void setWindowName(std::string name) {window_name = name;}
		int init();
		int run();
		void handleInputs();
		void update();
		void draw();
		void drawToScreen();
		void close();
		bool *getRunningPointer() {return &isRunning;}
};
