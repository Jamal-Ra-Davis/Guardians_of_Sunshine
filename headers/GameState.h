#pragma once
#include <SDL2/SDL.h>
#include <list>

#include "../headers/GameStateManager.h"
#include "../headers/Background.h"
#include "../headers/TileMap.h"
#include "../headers/Enemy.h"
#include "../headers/Explosion.h"
#include "../headers/AudioPlayer.h"
#include "../headers/BgObject.h"
#include "../headers/GoS_Player.h"
#include "../headers/Coin.h"
#include "../headers/TriggerBox.h"
#include "../headers/GoS_HUD.h"
#include "../headers/BouncyBee.h"
#include "../headers/Poof.h"
#include "../headers/HoneyBunny.h"
#include "../headers/SleepySam.h"
#include "../headers/ComboMove.h"
#include "../headers/Letters.h"
#include "../headers/GoS_StartMenu.h"

#define NUM_OPTIONS 2
class GameStateManager;
class GameState
{
	protected:
		GameStateManager *gsm;
		SDL_Renderer *renderTarget;

	public:
		virtual void init() =0;
		virtual void update() =0;
		virtual void draw() =0;
		virtual void keyPressed(int k) =0;
		virtual void keyReleased(int k) =0;
};

class MenuState : public GameState
{
	private:
		char const *options[3];
		SDL_Texture *title;
		SDL_Texture *option_textures[NUM_OPTIONS];
		int currentChoice;
		Background *bg;		

	public:
		MenuState(GameStateManager *gsm_, SDL_Renderer *renderTarget_);
		~MenuState();
		void init();
		void update();
		void draw();
		void keyPressed(int k);
		void keyReleased(int k);
};

class Level1State : public GameState
{
	private:
		TileMap *tileMap;
		TileMap *bgTileMap;
		Background *bg;
		//Player *player;
		GoS_Player *gos_player;
		BouncyBee *bouncyBee;
		SleepySam *sleepySam;
		ComboMove *comboMove;

		std::list<Enemy*> enemies;
		std::list<Explosion*> explosions;
		//HUD *hud;
		GoS_HUD *gos_hud;		
		GoS_StartMenu *gos_startmenu;	

		BgObject *sun, *slime;
		BgObject *fire[3];

		std::list<Coin*> coins;

		std::list<Poof*> enemy_poofs;

		int pstartX, pstartY;
		int sleepySamX, sleepySamY;
		int score;
		int lives;
		bool game_over;
		bool game_win;
		bool player_died;
		bool level_start;
		bool show_level_start;

		bool reachedBouncyBee;
		bool reachedHoneyBunny;
		bool reachedSleepySam;


		bool you_lose;
		double screen_timer;

		//TriggerBox *triggerBoxes[4];
		TriggerBox *bouncyBeeTrig, *honeyBunnyTrig, *sleepySamTrig;
		TriggerBox *fireBox0, *fireBox1, *fireBox2;
		TriggerBox *pitBox0, *pitBox1;
		std::list<TriggerBox*> triggerBox_list;	

		SDL_Texture *preScaleTexture;// = NULL;
    	SDL_Texture *gameWorldTexture;// = NULL;
		SDL_Texture *loseScreenTexture;
		SDL_Texture *level1ScreenTexture;
	

		bool trackPlayerY;
		int playerYLock;

		AudioPlayer *bgm;
		AudioPlayer **level_sounds;


		void populateEnemies();
		void populateCoins();
		void reset();
		void updateGameCamera();
	public:
		enum Level_Sounds{POOF_SFX, LEVEL_START_SFX, LEVEL_LOSE_SFX, RESPAWN_SFX, SFX_NUM};
		Level1State(GameStateManager *gsm_, SDL_Renderer *renderTarget_);
		~Level1State();
		void init();
        void update();
        void draw();
        void keyPressed(int k);
        void keyReleased(int k);
};

class TitleState : public GameState
{
	private:
		Animation titleAnimation;
		SDL_Texture *animationTexture;
        SDL_Rect *sprite_rects;
        int numSprites;
		//BgObject *titleScreen;
		AudioPlayer *bgm;
	
	public:
		TitleState(GameStateManager *gsm_, SDL_Renderer *renderTarget_);
		~TitleState();
		void init();
        void update();
        void draw();
        void keyPressed(int k);
        void keyReleased(int k);
};
class WinState : public GameState
{
	private:
		double xScene, yScene;
		int yFinal;
		int state;

		SDL_Texture *preScaleTexture;

		Animation sunAnimation;
		SDL_Texture *sunTexture;
		SDL_Rect *sun_rects;
		int numSunSprites;
		int xSun, ySun;
		int wSun, hSun; 

		Animation bmoAnimation;
		SDL_Texture *bmoTexture;
		SDL_Rect *bmo_rects;
		int numBmoSprites;
		int xBmo, yBmo;
		int wBmo, hBmo;

		Letters **text0;
		Letters **text1;//Score text
		Letters **text2;

		int text0_size, text1_size, text2_size;
		int *text0_delays;
		int counter;		

		Numbers *score;
		int score_val;		

		AudioPlayer **textSFX;
			
		//AudioPlayer *text_Fin_SFX;		
		double timer;
		bool animation_flag;

	public:
		enum State{START, PAN, FINAL};
		enum Text_Sounds{TEXT_START_SFX, TEXT_MID_SFX, TEXT_END_SFX, SFX_NUM};
		WinState(GameStateManager *gsm_, SDL_Renderer *renderTarget_);
        ~WinState();
        void init();
        void update();
        void draw();
        void keyPressed(int k);
        void keyReleased(int k);
};
