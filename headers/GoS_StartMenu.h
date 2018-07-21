#pragma once
#include <SDL2/SDL.h>
//#include "../headers/GoS_Player.h"
//#include "../headers/SDL_Helpers.h"
#include "../headers/Letters.h"
#include "../headers/Animation.h"

class GoS_StartMenu
{
	private:
		SDL_Texture *menuBg;
        SDL_Rect menuBgRect;
	
		SDL_Texture *comboIcon;
        SDL_Rect comboRect;
		int x_offset;
		int y_offset;
		int x_spacing;


		Letters* combo_text;

		Letters* controls_jump;
		Letters* controls_walk;
		Letters* controls_kick;
		Letters* controls_bomba;		

		SDL_Texture *player_texture;
		SDL_Rect playerRect;
		SDL_Rect cropRect;

		//Animations
		int width;
		int height;
        int numAnimations;
        //SDL_Texture *animationTexture;
        SDL_Rect **sprite_rects;
        int *frameNumbers;

		int currentAction_jump;
		Animation jump_anim;

		int currentAction_walk;
		Animation walk_anim;
	
		int currentAction_kick;
        Animation kick_anim;	

		int currentAction_bomba;
        Animation bomba_anim;
        //SDL_Rect bombaRect;

		

		bool visible;
		bool allow_visible;
	
		SDL_Renderer *renderTarget;
		//Numbers *scoreTracker;

	public:
		GoS_StartMenu(SDL_Renderer *renderTarget_, int x=0, int y=20);
		~GoS_StartMenu();
		
		void setVisible(bool v) {visible = v;}
		bool getVisible() {return visible;}
		void setAllowVisible(bool v) {allow_visible = v;}
		bool getAllowVisible() {return allow_visible;}
		bool getTrueVisible() {return (allow_visible &&	visible);}
		void update();
		void update_jump();
		void update_walk();
		void update_kick();
		void update_bomba();
		void setMenuPosition(int x, int y) {menuBgRect.x = x; menuBgRect.y = y;}
        void draw();
};
