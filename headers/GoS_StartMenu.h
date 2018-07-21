#pragma once
#include <SDL2/SDL.h>
//#include "../headers/GoS_Player.h"
//#include "../headers/SDL_Helpers.h"
//#include "../headers/Numbers.h"


class GoS_StartMenu
{
	private:
		SDL_Texture *menuBg;
        SDL_Rect menuBgRect;
	
		bool visible;
		//SDL_Texture *playerIcon;
        //SDL_Rect playerRect;

		//SDL_Texture *bombaIcon;
        //SDL_Rect bombaRect;
	
		SDL_Renderer *renderTarget;
		//Numbers *scoreTracker;

	public:
		GoS_StartMenu(SDL_Renderer *renderTarget_, int x=0, int y=20);
		~GoS_StartMenu();
		
		void setVisible(bool v) {visible = v;}
		bool getVisible() {return visible;}	
		void update();
		void setMenuPosition(int x, int y) {menuBgRect.x = x; menuBgRect.y = y;}
        void draw();
};
