#include "../headers/GameState.h"
#include "../headers/SDL_Helpers.h"
#include "../headers/GamePanel.h"
#include "../headers/TileMap.h"
#include "../headers/Slugger.h"
#include "../headers/InputBuffer.h"

MenuState::MenuState(GameStateManager *gsm_, SDL_Renderer *renderTarget_) 
{
	gsm = gsm_;
	renderTarget = renderTarget_;
	currentChoice = 0;
	options[0] = "Start";
	options[1] = "Help";
	options[2] = "Quit";

	bg = NULL;
	bg = new Background("./Resources/Backgrounds/menubg.bmp", 1, renderTarget);
	if (bg == NULL)
		printf("Failed to create background\n");
	bg->setVector(-0.1, 0);

	title = NULL;
	title = LoadTexture("./Resources/HUD/Title.bmp", renderTarget);


	for (int i=0; i<NUM_OPTIONS; i++)
		option_textures[i] = NULL;

	option_textures[0] = LoadTexture("./Resources/HUD/Start.bmp", renderTarget);
	option_textures[1] = LoadTexture("./Resources/HUD/Quit.bmp", renderTarget);
}
MenuState::~MenuState()
{
	if (bg)
		delete bg;
	bg = NULL;

	if (title)
		SDL_DestroyTexture(title);
	title = NULL;

	for (int i=0; i<NUM_OPTIONS; i++)
	{
		if (option_textures[i])
			SDL_DestroyTexture(option_textures[i]);
		option_textures[i] = NULL;
	}
}
void MenuState::init()
{

}
void MenuState::update()
{
	bg->update();
}
void MenuState::draw()
{
	bg->draw();

	//Draw Title
	SDL_Rect posRect;
	posRect.x = 40;
	posRect.y = 50;
	SDL_QueryTexture(title, NULL, NULL, &posRect.w, &posRect.h);	
	SDL_RenderCopy(renderTarget, title, NULL, &posRect);

	//Draw Menu options
	posRect.x += 60;
    posRect.y += 60;
	for (int i=0; i<NUM_OPTIONS; i++)
	{
		SDL_QueryTexture(option_textures[i], NULL, NULL, &posRect.w, &posRect.h);
		if (currentChoice == i)
		{
			SDL_SetTextureColorMod(option_textures[i], 255, 0, 0);
		}
		else
		{
			SDL_SetTextureColorMod(option_textures[i], 255, 255, 255);
		}
		SDL_RenderCopy(renderTarget, option_textures[i], NULL, &posRect);
		posRect.y += 30;
	}
}
void MenuState::keyPressed(int k)
{
	switch (k)
	{
		case SDLK_UP:
		{
			currentChoice--;
			if (currentChoice < 0)
			{
				currentChoice = NUM_OPTIONS - 1;
			}
			break;
		}
		case SDLK_DOWN:
		{
			currentChoice++;
            if (currentChoice >= NUM_OPTIONS)
            {
                currentChoice = 0;
            }
			break;
		}
		case SDLK_RETURN:
		{
			if (currentChoice == 0)
			{
				gsm->setState(GameStateManager::LEVEL1_STATE);
			}
			else if (currentChoice == 1)
			{
				//exit(0);
				if (GamePanel::isRunningControl == NULL)
					printf("isRunning cotrol is NULL\n");
				*(GamePanel::isRunningControl) = false;
			}
			break;
		}
	}
}
void MenuState::keyReleased(int k)
{

}


//-------------------------------------

Level1State::Level1State(GameStateManager *gsm_, SDL_Renderer *renderTarget_)
{
	gsm = gsm_;
    renderTarget = renderTarget_;
	tileMap = NULL;
	bgTileMap = NULL;
	bg = NULL;
	gos_player = NULL;
	//bouncyBee = NULL;
	sleepySam = NULL;
	comboMove = NULL;

	bgm = NULL;
	level_sounds = NULL;

	score = 0;
	lives = 2;
	game_over = false;
    player_died = false;
	trackPlayerY = false;

	//Trigger boxes
	bouncyBeeTrig = NULL;
	honeyBunnyTrig = NULL;
	sleepySamTrig = NULL;
	fireBox0 = NULL;
	fireBox1 = NULL;
	fireBox2 = NULL;
	pitBox0 = NULL;
	pitBox1 = NULL;


	gos_hud = NULL;
	gos_startmenu = NULL;

	preScaleTexture = NULL;
	gameWorldTexture = NULL;
	loseScreenTexture = NULL;
	level1ScreenTexture = NULL;	

	game_over = false;
	game_win = false;
	player_died = false;
	level_start = true;
	show_level_start = false;
	reachedBouncyBee = false;
	reachedHoneyBunny = false;
	reachedSleepySam = false;
	you_lose = false;

	//init();
}
Level1State::~Level1State()
{
	if (preScaleTexture)
		SDL_DestroyTexture(preScaleTexture);
	preScaleTexture = NULL;
	if (gameWorldTexture)
        SDL_DestroyTexture(gameWorldTexture);
    gameWorldTexture = NULL;
	if (loseScreenTexture)
        SDL_DestroyTexture(loseScreenTexture);
    loseScreenTexture = NULL;
	if (level1ScreenTexture)
        SDL_DestroyTexture(level1ScreenTexture);
    level1ScreenTexture = NULL;

	if (tileMap)
		delete tileMap;
	tileMap = NULL;

	if (bgTileMap)
		delete bgTileMap;
	bgTileMap = NULL;

	if (bg)
		delete bg;
	bg = NULL;

	if (gos_player)
        delete gos_player;
    gos_player = NULL;

	//if(bouncyBee)
	//	delete bouncyBee;
	//bouncyBee = NULL;

	

	if (bgm)
		delete bgm;
	bgm = NULL;

	for (int i=0; i<SFX_NUM; i++)
    {
		if (level_sounds[i])
        	delete level_sounds[i];
        level_sounds[i] = NULL;
    }
    delete level_sounds;
    level_sounds = NULL;



	if (sun)
		delete sun;
	sun = NULL;
	
	if (slime)
		delete slime;
	slime = NULL;

	for (int i=0; i<3; i++)
	{
		if (fire[i])
			delete fire[i];
		fire[i] = NULL;
	}




	if (gos_hud)
		delete gos_hud;
	gos_hud = NULL;

	if (gos_startmenu)
		delete gos_startmenu;
	gos_startmenu = NULL;

	//Delete elements from lists
}

void Level1State::init()
{

	pstartX = 70;
	pstartY = 105;

	sleepySamX = 2374;
	sleepySamY = 262 + 2;
	
	score = 0;
	lives = 2;

	game_over = false;
    game_win = false;
    player_died = false;
    level_start = true;
	show_level_start = false;
	reachedBouncyBee = false;
	reachedHoneyBunny = false;
    reachedSleepySam = false;
    you_lose = false;
	
	//tileMap = new TileMap(30, renderTarget);
	tileMap = new TileMap(12, renderTarget);
	//tileMap->loadTiles("./Resources/Tilesets/grasstileset.bmp");
	tileMap->loadTiles("./Resources/Tilesets/GoS_Tiles.bmp", "./Resources/Maps/GoS_TileTypes.txt");
	//tileMap->loadMap("./Resources/Maps/level1-1.map");
	tileMap->loadMap("./Resources/Maps/GoS_Level.map");
	tileMap->setPosition(0, 0);
	tileMap->setTween(1);

	bgTileMap = new TileMap(74, renderTarget);
	bgTileMap->loadTiles("./Resources/Tilesets/GoS_BG_Tiles.bmp", "");
	bgTileMap->loadMap("./Resources/Maps/GoS_BG.map");
	bgTileMap->setPosition(0, -66);
	bgTileMap->setTween(1);
	bgTileMap->setFixBounds(false);

	bg = new Background("./Resources/Backgrounds/grassbg1.bmp", 0.1, renderTarget);
	

	gos_player = new GoS_Player(tileMap, renderTarget);
    gos_player->setPosition(pstartX, pstartY);//75, 0
    gos_player->setVector(0, 0);

	comboMove = new ComboMove(tileMap, renderTarget);
	comboMove->setPosition(pstartX, pstartY);
	comboMove->setPosition(sleepySamX - 21, sleepySamY-2);
	comboMove->setVector(0,0);
	//bouncyBee = new BouncyBee(tileMap, renderTarget);
	//bouncyBee->setPosition(1000, 154 - 8);
	//bouncyBee->setVector(0, 0);	
	//bouncyBee->setTarget(830, 154);


	sun = NULL;
	slime = NULL;
	for (int i=0; i<3; i++)
		fire[i] = NULL;

	sun = new BgObject(tileMap, renderTarget, 0);
	slime = new BgObject(tileMap, renderTarget, 1);
	for (int i=0; i<3; i++)
		fire[i] = new BgObject(tileMap, renderTarget, 2);
	
	sun->setPosition(70, 60);
	sun->setVector(0, 0);

	slime->setPosition(1555, 107);
	slime->setVector(0, 0);

	fire[0]->setPosition(420+18, 216-18);
    fire[0]->setVector(0, 0);
	fire[1]->setPosition(2232+18, 324-18);
    fire[1]->setVector(0, 0);
	fire[2]->setPosition(2292+18, 324-18);
    fire[2]->setVector(0, 0);


	populateCoins();

	//triggerBoxes[0] = new TriggerBox(420+18, 216-4, 40, 30, true);//Fire pit 0 trigger box
	//triggerBoxes[1] = new TriggerBox(110, 105, 6, 60, false);
	

	//triggerBoxes[2] = new TriggerBox(1500, 154, 6, 60, true);//Bouncy bee trigger box
	
	//triggerBoxes[3] = new TriggerBox(794, 154, 6, 60, true);//Honey bunny trigger box

	bouncyBeeTrig = new TriggerBox(794, 154, 6, 60, false);
	honeyBunnyTrig = new TriggerBox(1520, 154, 6, 60, false);
    sleepySamTrig = new TriggerBox(2340, 250, 6, 60, false);

    fireBox0 = new TriggerBox(420+18, 216-4, 40, 30, false);
    fireBox1 = new TriggerBox(2232+18, 324-4, 40, 30, false);
    fireBox2 = new TriggerBox(2292+18, 324-4, 40, 30, false);

    pitBox0 = new TriggerBox(1072+20, 202 + 16, 40, 10, false);
    pitBox1 = new TriggerBox(1142+20, 202 + 16, 40, 10, false);

	triggerBox_list.push_back(bouncyBeeTrig);
	triggerBox_list.push_back(honeyBunnyTrig);
	triggerBox_list.push_back(sleepySamTrig);
	triggerBox_list.push_back(fireBox0);
	triggerBox_list.push_back(fireBox1);
	triggerBox_list.push_back(fireBox2);
	triggerBox_list.push_back(pitBox0);
	triggerBox_list.push_back(pitBox1);

	trackPlayerY = false;
	playerYLock = 105;

	gos_hud = new GoS_HUD(renderTarget, 3, 3);
	gos_startmenu = new GoS_StartMenu(renderTarget, 8, 8);

	populateEnemies();

	bgm = new AudioPlayer("./Resources/Music/BMOGame_LevelLoop_Louder.ogg", true);
	level_sounds = NULL;
    level_sounds = new AudioPlayer*[SFX_NUM];
    for (int i=0; i<SFX_NUM; i++)
    {
        level_sounds[i] = NULL;
    }
	level_sounds[POOF_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_EnemyDies.wav", false);
	level_sounds[LEVEL_START_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_LevelStart.wav", false);
	level_sounds[LEVEL_LOSE_SFX] = new AudioPlayer(/*"./Resources/SFX/BMOGame_Death.wav"*/"./Resources/SFX/BMOGame_YouLose.wav", false);
	level_sounds[RESPAWN_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_Respawn.wav", false);
	

	//gameWorldTexture = SDL_CreateTexture(renderTarget, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, GamePanel::WIDTH, GamePanel::HEIGHT);
    preScaleTexture = SDL_CreateTexture(renderTarget, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 206, 152);
    gameWorldTexture = SDL_CreateTexture(renderTarget, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 206, 116);

	level1ScreenTexture = LoadTexture("./Resources/Sprites/GoS_Level1_Text.bmp", renderTarget);
	loseScreenTexture = LoadTexture("./Resources/Sprites/GoS_Game_Over_Text.bmp", renderTarget);
	//Display Level 1 Screen?


	//Start level stuff
	bgm->play(AudioPlayer::LOOP);	
	screen_timer = getMs();


}
void Level1State::update()
{
	//if (player_died)
	if (level_start)
	{
		if (getMs() - screen_timer > 500 && !show_level_start)
		{
			show_level_start = true;
			level_sounds[LEVEL_START_SFX]->play(0);
		}
		if (getMs() - screen_timer > 2500)
			level_start = false;
		else
			return;
	}
	

	if (game_over)
	{
		printf("Game over happened\n");
		*(GamePanel::isRunningControl) = false;
		bgm->stop();
		return;
	}
	if (game_win)
	{
		printf("Won game\n");
		bgm->stop();
		gsm->setScore(score);
		gsm->setState(GameStateManager::WIN_STATE);
		return;
	}	
	
	if (gos_player->isDead() && !player_died)
	{
		player_died = true;
		screen_timer = getMs();
		bgm->stop();
		level_sounds[LEVEL_LOSE_SFX]->play(0);
		//lives--;
		//if (lives < 0)
		//{
		//	lives = 0;
		//	game_over = true;
		//}
		
		//reset();
	}
	if (player_died)
	{
		if (getMs() - screen_timer >= 3000/*4000*/)
		{
			lives--;
			if (lives < 0)
        	{
				printf("Game over should happend\n");
            	lives = 0;
            	game_over = true;
				return; 
        	}
			level_sounds[RESPAWN_SFX]->play(0);
			reset();
		}
		//printf("YOU LOSE!!\n");
		return;
	}
	
	//player_died = false;
	//printf("Score: %d\n", score);
	//player->update();

	if (sleepySamTrig->wasTripped() && gos_player->isOnGround())
	{
		if (!reachedSleepySam)
		{
			if (gos_player->getX() >= sleepySamX - 21)
			{
				reachedSleepySam = true;
				gos_player->setRight(false);
				gos_player->setPosition(sleepySamX - 21, gos_player->getY());
				gos_player->setHold(true);
				gos_player->setVisible(false);
				
				comboMove->setActive(true);
				comboMove->setVisible(true);
				printf("Reached sleepy sam\n");
			}
			else
			{
				gos_player->setRight(true);
				gos_player->setLeft(false);
				gos_player->setJumping(false);
			}
		}
	}
	//printf("Player Hold: %d\n", gos_player->getHold());
	gos_player->update();
	comboMove->update();
	if (comboMove->comboFailed())
	{
		sleepySam->setEatPlayer(true);
	}
	if (comboMove->comboPassed() && !sleepySam->isDead())
	{
		sleepySam->setDead(true);
		screen_timer = getMs();
	}
	if (comboMove->comboPassed())
	{
		if (getMs() - screen_timer > 3000)
		{
			game_win = true;
		}	
	}
	/*
	if ((triggerBoxes[1]->intersectsObject(gos_player) || triggerBoxes[2]->intersectsObject(gos_player)))
	{
		//trackPlayerY = true;
		triggerBoxes[0]->reset();
		triggerBoxes[3]->reset();
	}
	else if((triggerBoxes[0]->intersectsObject(gos_player) || triggerBoxes[3]->intersectsObject(gos_player)))
	{
		//playerYLock = gos_player->getY();
		//trackPlayerY = false;	
		triggerBoxes[1]->reset();
        triggerBoxes[2]->reset();
	}
	if (gos_player->isOnGround())
	{
		if (triggerBoxes[1]->wasTripped() || triggerBoxes[2]->wasTripped())
		{
			trackPlayerY = true;
		}
		else if (triggerBoxes[0]->wasTripped() || triggerBoxes[3]->wasTripped())
		{
			playerYLock = gos_player->getY();
        	trackPlayerY = false;
		}
	}
	*/
	if (gos_player->isOnGround())
	{
		playerYLock = gos_player->getY();
	}
	updateGameCamera();	
	/*
	double x = tileMap->getX();
    double y = tileMap->getY();
    double x1 = bgTileMap->getX() - x;
    double y1 = bgTileMap->getY() - y;
    bgTileMap->setPosition((int)(tileMap->getX()+0), (int)(tileMap->getY()-66));
	*/

	//for (int i=0; i<4; i++)
	//	triggerBoxes[i]->intersectsObject(gos_player);

	std::list<TriggerBox*>::iterator trig_iter;
    for (trig_iter=triggerBox_list.begin(); trig_iter != triggerBox_list.end(); ++trig_iter)
    {
		TriggerBox *triggerBox = (*trig_iter);
     	triggerBox->intersectsObject(gos_player);
    }


	//if (triggerBoxes[0]->intersectsObject(gos_player))
	//{
	//	player_died = true;
	//}
	if (fireBox0->intersectsObject(gos_player) || fireBox1->intersectsObject(gos_player) || fireBox2->intersectsObject(gos_player)
		 || pitBox0->intersectsObject(gos_player) || pitBox1->intersectsObject(gos_player))
	{
		gos_player->setDead(true);
		////player_died = true;
	}	
	/*
	if (triggerBoxes[3]->wasTripped())
	{
		bouncyBee->setActive(true);
	}
	bouncyBee->update();
	*/


	//if (gos_player->intersects(coin) && !coin->isCollected())
	//{
	//	coin->collect();
	//}
	//printf("(X,Y) = %d, %d\n", (int)gos_player->getX(), (int)gos_player->getY());

	sun->update();
	slime->update();

	std::list<Coin*>::iterator iter;
	for (iter=coins.begin(); iter != coins.end(); /*++iter*/)
    {
        Coin *coin_ = (*iter);
        coin_->update();
		if (gos_player->intersects(coin_) && !coin_->isCollected())
		{
			coin_->collect();
			score += coin_->getValue();
		}
        if (coin_->shouldRemove())
        {
            delete coin_;
            *iter = NULL;
            iter = coins.erase(iter);

        }
        else
        {
            ++iter;
        }
    }


	for (int i=0; i<3; i++)
		fire[i]->update();
	//tileMap->setPosition(GamePanel::WIDTH/2 - player->getX(),
	//					 GamePanel::HEIGHT/2 - player->getY());

	/*
	if (trackPlayerY)
	{
		tileMap->setPosition(GamePanel::WIDTH/2 - gos_player->getX() - 18,
        	               GamePanel::HEIGHT/2 - gos_player->getY() + 12);
	}
	else
	{
		tileMap->setPosition(GamePanel::WIDTH/2 - gos_player->getX() - 18,
                           GamePanel::HEIGHT/2 - playerYLock + 12);
	}
	*/

/*
	double x = tileMap->getX();
    double y = tileMap->getY();
    double x1 = bgTileMap->getX() - x;
    double y1 = bgTileMap->getY() - y;
	bgTileMap->setPosition((int)(tileMap->getX()+0), (int)(tileMap->getY()-66));
*/
/*
	bg->setPosition(tileMap->getX(), tileMap->getY());

	player->checkAttack(enemies);
	if (player->getY() > 400 || player->isDead())
    {
        *(GamePanel::isRunningControl) = false;
    }	
*/
	gos_player->checkAttack(enemies);

	if (!reachedSleepySam)
		gos_player->setHold(false);
	std::list<Enemy*>::iterator enemies_iter;
    for (enemies_iter=enemies.begin(); enemies_iter != enemies.end(); )
    {
		Enemy *enemy = (*enemies_iter);
        enemy->update();
		TriggerBox *eTrigBox = enemy->getTriggerBox();
		if (eTrigBox)
		{
			if (eTrigBox->wasTripped() && !enemy->isReady())
			{
				gos_player->setHold(true);
				if (!gos_player->isOnGround() && gos_player->getX() >= eTrigBox->getPosX())
				{
					gos_player->setPosition(eTrigBox->getPosX(), gos_player->getY());
				}
			}
		}
		if (enemy->isDead())
		{
			double xPos, yPos;
			enemy->getPoofPosition(&xPos, &yPos);
			//int xPos = (int)enemy->getX();
			//int yPos = (int)enemy->getY();
			int eScore = enemy->getScore();
			score += eScore;
			delete enemy;
			*enemies_iter = NULL;
			enemies_iter = enemies.erase(enemies_iter);
			enemy_poofs.push_back(new Poof(renderTarget, (int)xPos, (int)yPos, eScore));
			level_sounds[POOF_SFX]->play(0);
		}
		else
		{
			++enemies_iter;
		}
    }
	sleepySam->update();
	if (sleepySam->getFinishedEating())
	{
		gos_player->setDead(true);	
	}
	if (sleepySam->isDead() && !sleepySam->getPlayedPoof())
	{
		double xPos, yPos;
		sleepySam->getPoofPosition(&xPos, &yPos);
		int eScore = sleepySam->getScore();
		score += eScore;
		sleepySam->setVisible(false);
		sleepySam->setPlayedPoof(true);
		enemy_poofs.push_back(new Poof(renderTarget, (int)xPos, (int)yPos, eScore));
		level_sounds[POOF_SFX]->play(0);
	}

	std::list<Poof*>::iterator poofs_iter;
    for (poofs_iter=enemy_poofs.begin(); poofs_iter != enemy_poofs.end(); )
    {
        Poof *poof = *poofs_iter;
		poof->update();
		if (poof->shouldRemove())
		{
			delete poof;
			*poofs_iter = NULL;
			poofs_iter = enemy_poofs.erase(poofs_iter);
		}
		else
		{
			++poofs_iter;
		}
    }
/*	
	std::list<Explosion*>::iterator explosions_iter;
	for (explosions_iter=explosions.begin(); explosions_iter != explosions.end(); )
	{
		Explosion *explosion = *explosions_iter;
		explosion->update();
		if (explosion->shouldRemove())
		{
			delete explosion;
			*explosions_iter = NULL;
			explosions_iter = explosions.erase(explosions_iter);
		}
		else
		{
			++explosions_iter;
		}
	}
*/

}
void Level1State::draw()
{
	SDL_DisplayMode current;
	SDL_GetDesktopDisplayMode(0, &current);
	int window_width, window_height;
	//SDL_GetWindowSize()
/*
	SDL_Texture *preScaleTexture = NULL;
	SDL_Texture *gameWorldTexture = NULL;

	//gameWorldTexture = SDL_CreateTexture(renderTarget, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, GamePanel::WIDTH, GamePanel::HEIGHT);
	preScaleTexture = SDL_CreateTexture(renderTarget, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 206, 152);
	gameWorldTexture = SDL_CreateTexture(renderTarget, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 206, 116);
*/
	//SDL_Rect scaleRect = {11, 11+110, 3*206, 3*116};

	
	//Setup to render to gameWorldTexture instead of final screen
	SDL_SetRenderTarget( renderTarget, gameWorldTexture );


	//Draw Background
	//bg->draw();

	//Draw Tilemap

	//Background and background objects
	bgTileMap->draw();
	sun->draw();
	slime->draw();

	//Player
	gos_player->draw();
    comboMove->draw();

	std::list<Coin*>::iterator iter;
    for (iter=coins.begin(); iter != coins.end(); ++iter)
    {
        (*iter)->draw();
    }

	for (int i=0; i<3; i++)
        fire[i]->draw();	
	tileMap->draw();

	//Draw Player
	//printf("DEBUG: about to draw player\n");
	//for (int i=0; i<4; i++)
	//{
	//	triggerBoxes[i]->draw(tileMap, renderTarget);
	//}
	std::list<TriggerBox*>::iterator trig_iter;
	for (trig_iter=triggerBox_list.begin(); trig_iter != triggerBox_list.end(); ++trig_iter)
	{
		(*trig_iter)->draw(tileMap, renderTarget);
	}

	//player->draw();
	//gos_player->draw();
	//comboMove->draw();
	//honeyBunny->draw();
	//bouncyBee->draw();
	sleepySam->draw();
	std::list<Enemy*>::iterator enemies_iter;
    for (enemies_iter=enemies.begin(); enemies_iter != enemies.end(); ++enemies_iter)
    {
        (*enemies_iter)->draw();
    }
	std::list<Poof*>::iterator poofs_iter;
    for (poofs_iter=enemy_poofs.begin(); poofs_iter != enemy_poofs.end(); ++poofs_iter)
    {
        Poof *poof = *poofs_iter;
        poof->setMapPosition((int)tileMap->getX(), (int)tileMap->getY());
        poof->draw();
    }

	gos_startmenu->draw();



	//Setup to render to preScaleTexture instead of final screen
    SDL_SetRenderTarget( renderTarget, preScaleTexture );
	SDL_Rect gameWorldRect = {0, 36, 206, 116};
	SDL_Rect gameWorldCrop = {206/4, 116/4+6, 206/2, 116/2};
	SDL_RenderClear(renderTarget);
	if (!reachedSleepySam)
	{
		SDL_RenderCopy(renderTarget, gameWorldTexture, NULL, &gameWorldRect);
	}
	else
		SDL_RenderCopy(renderTarget, gameWorldTexture, &gameWorldCrop, &gameWorldRect);
	//Draw HUD
	gos_hud->draw(score, lives, gos_player->getBombaCount());
	
	if (player_died)
	{
		SDL_RenderClear(renderTarget);
		SDL_Rect loseScreenRect;
		SDL_QueryTexture(loseScreenTexture, NULL, NULL, &(loseScreenRect.w), &(loseScreenRect.h));
		loseScreenRect.x = 206/2 - loseScreenRect.w/2;
		loseScreenRect.y = 152/2 - loseScreenRect.h/2;
		SDL_RenderCopy(renderTarget, loseScreenTexture, NULL, &loseScreenRect);
	}
	if (level_start)
	{
		SDL_RenderClear(renderTarget);
		if (show_level_start)
		{
        	SDL_Rect level1ScreenRect;
        	SDL_QueryTexture(level1ScreenTexture, NULL, NULL, &(level1ScreenRect.w), &(level1ScreenRect.h));
        	level1ScreenRect.x = 206/2 - level1ScreenRect.w/2;
        	level1ScreenRect.y = 152/2 - level1ScreenRect.h/2;
        	SDL_RenderCopy(renderTarget, level1ScreenTexture, NULL, &level1ScreenRect);
		}
	}

	//Set renderer back to main window
	SDL_SetRenderTarget( renderTarget, NULL );
	//SDL_Rect preScaleRect = {11, 12, 206*3, 152*3};

	int width_N = GamePanel::WINDOW_WIDTH/206;
	//int width_R = GamePanel::WINDOW_WIDTH%206;
	int height_N = GamePanel::WINDOW_HEIGHT/152;
	//int height_R = GamePanel::WINDOW_HEIGHT%152;
	int scale = width_N;
	if (width_N > height_N)
    {
        scale = height_N;
    }
	int width_R = GamePanel::WINDOW_WIDTH - 206*scale;
    int height_R = GamePanel::WINDOW_HEIGHT - 152*scale;	


	//SDL_Rect preScaleRect = {width_R/2, height_R/2, 206*width_N, 152*height_N};
	SDL_Rect preScaleRect = {width_R/2, height_R/2, 206*scale, 152*scale};
	SDL_RenderCopy(renderTarget, preScaleTexture, NULL, &preScaleRect);
	//SDL_RenderCopy(renderTarget, gameWorldTexture, NULL, &scaleRect);

/*
	SDL_DestroyTexture(gameWorldTexture);
	SDL_DestroyTexture(preScaleTexture);	
	gameWorldTexture = NULL;
	preScaleTexture = NULL;
	*/
}
void Level1State::populateEnemies()
{
/*
	int locations[4][2] = {
							{200, 100},
							{1525, 200},
							{1680, 190},
							{1800, 190}
						};

	for (int i=0; i<4; i++)
	{
		Slugger *s = new Slugger(tileMap, renderTarget);
    	s->setPosition(locations[i][0], locations[i][1]);
    	enemies.push_back(s);
	}
*/
	BouncyBee *bouncyBee_ = new BouncyBee(tileMap, renderTarget);
    bouncyBee_->setPosition(1000, 154 - 8);
    bouncyBee_->setVector(0, 0);
    bouncyBee_->setTarget(830, 154);
	//bouncyBee_->setTriggerBox(triggerBoxes[3]);
	bouncyBee_->setTriggerBox(bouncyBeeTrig);
	bouncyBee_->setScore(500);
	enemies.push_back(bouncyBee_);


	HoneyBunny *honeyBunny_ = new HoneyBunny(tileMap, renderTarget);
    honeyBunny_->setPosition(1555, 134);
    honeyBunny_->setVector(0, 0);
	//honeyBunny_->setTriggerBox(triggerBoxes[2]);
	honeyBunny_->setTriggerBox(honeyBunnyTrig);
	honeyBunny_->setScore(800);
	enemies.push_back(honeyBunny_);

	sleepySam = new SleepySam(tileMap, renderTarget);
	sleepySam->setPosition(sleepySamX, sleepySamY);//75, 0
    sleepySam->setVector(0, 0);	
	sleepySam->setTriggerBox(sleepySamTrig);
	sleepySam->setScore(999);
	sleepySam->setComboMove(comboMove);
}
void Level1State::populateCoins()
{
	int locations[8][2] = {
							{1068+6, 130},
							//{1092 ,130},
							{1116-4, 130},
							{1128, 130},
							{1140+4, 130},
							//{1164, 130},
							{1188-1, 130},
							{1203, 130},
							{1219, 130},
							{1235, 130},
						};
	for (int i=0; i<8; i++)
	{
		Coin *c = new Coin(tileMap, renderTarget, 10);
		c->setPosition(locations[i][0], locations[i][1]);
		coins.push_back(c);
	}
}
void Level1State::reset()
{
	gos_player->reset(pstartX, pstartY);
	playerYLock = gos_player->getY();
	updateGameCamera();
	trackPlayerY = false;
	sleepySamTrig->reset();
	player_died = false;

	reachedSleepySam = false;
	sleepySam->reset();
	comboMove->reset();
	bgm->stop();
	bgm->play(AudioPlayer::LOOP);
	//gos_player->setPosition(70, 105);
    //gos_player->setVector(0, 0);	
}
void Level1State::updateGameCamera()
{
	if (trackPlayerY)
    {
        tileMap->setPosition(GamePanel::WIDTH/2 - gos_player->getX() - 18,
                           GamePanel::HEIGHT/2 - gos_player->getY() + 12);
    }
    else
    {
        tileMap->setPosition(GamePanel::WIDTH/2 - gos_player->getX() - 18,
                           GamePanel::HEIGHT/2 - playerYLock + 12);
    }
	double x = tileMap->getX();
    double y = tileMap->getY();
    double x1 = bgTileMap->getX() - x;
    double y1 = bgTileMap->getY() - y;
    bgTileMap->setPosition((int)(tileMap->getX()+0), (int)(tileMap->getY()-66));
}
void Level1State::keyPressed(int k)
{
	double x = tileMap->getX();
	double y = tileMap->getY();
	double x1 = bgTileMap->getX() - x;
	double y1 = bgTileMap->getY() - y;
	int scroll_val = -5;
	switch (k)
    {
        case SDLK_LEFT:
        {
			//player->setLeft(true);
			gos_player->setLeft(true);
			comboMove->setLeft(true);
			//x -= scroll_val;
			//x1 -= scroll_val;
            break;
        }
		case SDLK_RIGHT:
		{
			//player->setRight(true);
			gos_player->setRight(true);
			comboMove->setRight(true);
			//x += scroll_val;
			//x1 += scroll_val;
			break;
		}
		case SDLK_UP:
		{
			//player->setUp(true);
			gos_player->setUp(true);
			comboMove->setUp(true);
			//y -= scroll_val;
			//y1 -= scroll_val;
			break;
		}
		case SDLK_DOWN:
		{
			//player->setDown(true);
			gos_player->setDown(true);
			comboMove->setDown(true);
			//y += scroll_val;
			//y1 += scroll_val;
			break;
		}
		case SDLK_SPACE:
		{
			//player->setJumping(true);
			gos_player->setJumping(true);
			comboMove->setJumping(true);
			break;
		}
		case SDLK_LSHIFT:
		{
			//player->setGliding(true);
			break;
		}
		case SDLK_z:
		{
			//player->setScratching();
			gos_player->setKicking();
			comboMove->setSpin(true);
			break;
		}
		case SDLK_x:
		{
			//player->setFiring();
			gos_player->bombaAction();
			break;
		}
		case SDLK_RETURN:
		{	
			gos_startmenu->setVisible(true);
			for (int i=0; i<COMBO_LENGTH; i++)
				printf("%d, ", InputBuffer::data_test[i]);
			printf("\n");
			break;
		}
	}
	//tileMap->setPosition((int)x, (int)y);
	//printf("x: %d, y: %d\n", (int)tileMap->getX(), (int)tileMap->getY());
	//bgTileMap->setPosition((int)(tileMap->getX()+x1), (int)(tileMap->getY()+y1));
}
void Level1State::keyReleased(int k)
{
	switch (k)
    {
        case SDLK_LEFT:
        {
            //player->setLeft(false);
			gos_player->setLeft(false);
            break;
        }
        case SDLK_RIGHT:
        {
            //player->setRight(false);
			gos_player->setRight(false);
            break;
        }
        case SDLK_UP:
        {
            //player->setUp(false);
			gos_player->setUp(false);
            break;
        }
        case SDLK_DOWN:
        {
            //player->setDown(false);
			gos_player->setDown(false);
            break;
        }
        case SDLK_SPACE:
        {
            gos_player->setJumping(false);
            break;
        }
        case SDLK_LSHIFT:
        {
            //player->setGliding(false);
			bgm->stop();
			level_sounds[LEVEL_LOSE_SFX]->stop(-1);
			gsm->setState(GameStateManager::TITLE_STATE);
            break;
        }
		case SDLK_RSHIFT:
        {
			bgm->stop();
			level_sounds[LEVEL_LOSE_SFX]->stop(-1);
			*(GamePanel::isRunningControl) = false;
            break;
        }
		case SDLK_RETURN:
        {
            gos_startmenu->setVisible(false);
            break;
        }
    }
}




TitleState::TitleState(GameStateManager *gsm_, SDL_Renderer *renderTarget_)
{
	printf("Title screen constructor\n");
    gsm = gsm_;
    renderTarget = renderTarget_;
	animationTexture = NULL;
	sprite_rects = NULL;
	bgm = NULL;
    //init();
}
TitleState::~TitleState()
{
	if (sprite_rects)
		delete [] sprite_rects;
	sprite_rects = NULL;

	if (bgm)
		delete bgm;
	bgm = NULL;

	SDL_DestroyTexture(animationTexture);
    animationTexture = NULL;
}
void TitleState::init()
{
	printf("TItle screen init\n");
	int width = 640;
	int height = 480;
	animationTexture = NULL;
    animationTexture = LoadTexture("./Resources/Sprites/GoS_Title.bmp", renderTarget);
	if (!animationTexture)
		printf("Failed to load texture\n");
    numSprites = 2;
    sprite_rects = NULL;
    sprite_rects = new SDL_Rect[numSprites];
    for (int i=0; i<numSprites; i++)
    {
        sprite_rects[i].x = i*width;
        sprite_rects[i].y = 0;
        sprite_rects[i].w = width;
        sprite_rects[i].h = height;
    }
    titleAnimation.setFrames(animationTexture, sprite_rects, numSprites);
    titleAnimation.setDelay(666);

	//bgm = new AudioPlayer("./Resources/Music/BMOGame_Intro.mp3", true);
	//bgm->stop();
	//bgm->close();
	
    //bgm->play(0);
	//bgm = new AudioPlayer("./Resources/Music/level1-1.mp3", true);
	bgm = new AudioPlayer("./Resources/Music/BMOGame_Intro.mp3", true);
    //bgm->play(AudioPlayer::LOOP);
	bgm->play(0);
	//printf("at end of init title state\n");	
}
void TitleState::update()
{
	if (!bgm->isPlaying())
	{
		//bgm->play(AudioPlayer::LOOP);	
		int r = 0;
	}
	//else
	//	printf("BGM should be playing\n");
	titleAnimation.update();
}
void TitleState::draw()
{
	//int width_N = GamePanel::WINDOW_WIDTH/206;
    //int width_R = GamePanel::WINDOW_WIDTH%206;
    //int height_N = GamePanel::WINDOW_HEIGHT/152;
    //int height_R = GamePanel::WINDOW_HEIGHT%152;

	SDL_Rect posRect;

	double w_fract = GamePanel::WINDOW_WIDTH;
	w_fract /= sprite_rects[0].w;

	double h_fract = GamePanel::WINDOW_HEIGHT;
    h_fract /= sprite_rects[0].h;

	if (h_fract < w_fract)
	{
		SDL_Rect posRect_ = {0, 0, (int)(h_fract*sprite_rects[0].w), (int)(h_fract*sprite_rects[0].h)};
		posRect_.x = (GamePanel::WINDOW_WIDTH - posRect_.w)/2;
		posRect = posRect_;
	}
	else
	{
		SDL_Rect posRect_ = {0, 0, (int)(h_fract*sprite_rects[0].w), (int)(h_fract*sprite_rects[0].h)};
		posRect_.y = (GamePanel::WINDOW_HEIGHT - posRect_.h)/2;
        posRect = posRect_;
	}



	//SDL_Rect posRect = {0, 0, sprite_rects[0].w, sprite_rects[0].h};
    SDL_Rect cropRect = titleAnimation.getImageRect();
    SDL_RenderCopy(renderTarget, titleAnimation.getFrameTexture(), &cropRect, &posRect);
}
void TitleState::keyPressed(int k)
{
	switch(k)
	{
		case SDLK_RETURN:
		{	
			if (bgm)
			{
				bgm->stop();
				bgm->close();
			}
			gsm->setState(GameStateManager::LEVEL1_STATE);
			break;
		}
	}
}
void TitleState::keyReleased(int k)
{

}

//-------------------------------------------------

WinState::WinState(GameStateManager *gsm_, SDL_Renderer *renderTarget_)
{
	gsm = gsm_;
    renderTarget = renderTarget_;

	preScaleTexture = NULL;

	sunTexture = NULL;
	sun_rects = NULL;
	bmoTexture = NULL;
	bmo_rects = NULL;

	text0 = NULL;
	text1 = NULL;
	text2 = NULL;

	text0_delays = NULL;
	score = NULL;
	textSFX = NULL;
	//text_Fin_SFX = NULL;
}
WinState::~WinState()
{
	if (preScaleTexture)
		SDL_DestroyTexture(preScaleTexture);
	preScaleTexture = NULL;

	if (sunTexture)
		SDL_DestroyTexture(sunTexture);
	sunTexture = NULL;
	
	if (sun_rects)
		delete [] sun_rects;
	sun_rects = NULL;


	if (bmoTexture)
        SDL_DestroyTexture(bmoTexture);
    bmoTexture = NULL;

    if (bmo_rects)
        delete [] bmo_rects;
    bmo_rects = NULL;

	
	if (text0)
	{
		for (int i=0; i<text0_size; i++)
		{
			if (text0[i])
				delete [] text0[i];
			text0[i] = NULL;
		}
		text0 = NULL;
	}
	if (text1)
    {
        for (int i=0; i<text1_size; i++)
        {
            if (text1[i])
                delete [] text1[i];
            text1[i] = NULL;
        }
        text1 = NULL;
    }
	if (text2)
    {
        for (int i=0; i<text2_size; i++)
        {
            if (text2[i])
                delete [] text2[i];
            text2[i] = NULL;
        }
        text2 = NULL;
    }

	if (text0_delays)
		delete text0_delays;
	text0_delays = NULL;

	if (score)
		delete score;
	score = NULL;
	
	if (textSFX)
	{
		for (int i=0; i<SFX_NUM; i++)
		{
			if (textSFX[i])
				delete textSFX[i];
			textSFX[i] = NULL;
		}
		delete textSFX;
	}
	textSFX = NULL;
/*
	if (text_Fin_SFX)
        delete text_Fin_SFX;
    text_Fin_SFX = NULL;
*/
}
void WinState::init()
{
	preScaleTexture = SDL_CreateTexture(renderTarget, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 206, 152);

	xScene = 0;
	yScene = 0;
	yFinal = -200;
    state = START;

	wSun = 32;
	hSun = 36;
	xSun = 206/2 - wSun;
	ySun = 174;
	sunTexture = LoadTexture("./Resources/Sprites/GoS_SunSprites.bmp", renderTarget);
	numSunSprites = 2;
	sun_rects = new SDL_Rect[numSunSprites];
	for (int i=0; i<numSunSprites; i++)
    {
        //sprite_rects[i] = {i*width, 0, width, height};
        sun_rects[i].x = i*wSun;
        sun_rects[i].y = 0;
        sun_rects[i].w = wSun;
        sun_rects[i].h = hSun;
    }
    sunAnimation.setFrames(sunTexture, sun_rects, numSunSprites);
    sunAnimation.setDelay(333);

	xBmo = 206/8;
    yBmo = 260;
    wBmo = 57;
    hBmo = 63;
    bmoTexture = LoadTexture("./Resources/Sprites/GoS_BMO_Dance.bmp", renderTarget);
    numBmoSprites = 2;
    bmo_rects = new SDL_Rect[numBmoSprites];
    for (int i=0; i<numBmoSprites; i++)
    {
        //sprite_rects[i] = {i*width, 0, width, height};
        bmo_rects[i].x = i*wBmo;
        bmo_rects[i].y = 0;
        bmo_rects[i].w = wBmo;
        bmo_rects[i].h = hBmo;
    }
    bmoAnimation.setFrames(bmoTexture, bmo_rects, numBmoSprites);
    bmoAnimation.setDelay(300);
	

	text0_size = 5;
	text1_size = 1;
	text2_size = 3;//
	text0 = new Letters*[text0_size];
	text1 = new Letters*[text1_size];
	text2 = new Letters*[text2_size];

	
	text0[0] = new Letters(renderTarget, "CONGRATULATIONS", 3, 0, 1*152/5, false);
	text0[1] = new Letters(renderTarget, "YOU", 3, 0, 2*152/5, false);
	text0[2] = new Letters(renderTarget, "DEFENDED", 3, 0, 2*152/5, false);
	text0[3] = new Letters(renderTarget, "THE", 3, 0, 3*152/5, false);
	text0[4] = new Letters(renderTarget, "SUN", 3, 0, 3*152/5, false);




	int lineWidth = text0[0]->getWidth();
	text0[0]->setX(206/2 - lineWidth/2);
	lineWidth = text0[1]->getWidth() + text0[2]->getWidth() + 6 + 3;
	text0[1]->setX(206/2 - lineWidth/2);
	text0[2]->setX(206/2 - lineWidth/2 + text0[1]->getWidth() + 6 + 3);
	lineWidth = text0[3]->getWidth() + text0[4]->getWidth() + 6;
    text0[3]->setX(206/2 - lineWidth/2);
    text0[4]->setX(206/2 - lineWidth/2 + text0[3]->getWidth() + 6 + 3);
	

	text1[0] = new Letters(renderTarget, "SCORE", 3, 0, 260 + 8);
	lineWidth = text1[0]->getWidth();
	text1[0]->setX(82 + 62 - lineWidth/2);


	text2[0] = new Letters(renderTarget, "THANKS", 3, 0, 323 + 10);
	text2[1] = new Letters(renderTarget, "FOR", 3, 0, 323 + 10);
	text2[2] = new Letters(renderTarget, "PLAYING", 3, 0, 323 + 10);

	lineWidth = text2[0]->getWidth() + text2[1]->getWidth() + text2[2]->getWidth() + 2*(6+3);
	text2[0]->setX(206/2 - lineWidth/2);
	text2[1]->setX(206/2 - lineWidth/2 + text2[0]->getWidth() + 6 + 3);
	text2[2]->setX(206/2 - lineWidth/2 + text2[0]->getWidth() + 6 + 3 + text2[1]->getWidth() + 6 + 3);

	int temp_delays[5] = {2000, 1200, 600, 600, 600};
  	text0_delays = new int[text0_size];
	for (int i=0; i<text0_size; i++)
	{
		text0_delays[i] = temp_delays[i];
	}




   	score = new Numbers(renderTarget, 4, 3, 0, 260 + 24);
	lineWidth = score->getWidth();
	score->setX(82 + 62 - lineWidth/2);
	score_val = gsm->getScore();


    textSFX = new AudioPlayer*[SFX_NUM];
	textSFX[TEXT_START_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_LevelStart.wav", false);
	textSFX[TEXT_MID_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_Footstep.wav", false);
	textSFX[TEXT_END_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_Jump.wav", false);
    timer = -1;
	animation_flag = false;
	counter = 0;
	state = START;
}
void WinState::update()
{
	switch (state)
	{
		case START:
		{
			if (!animation_flag)
			{
				timer = getMs();
				animation_flag = true;
			}
			else
			{
				if (getMs() - timer >= text0_delays[counter])
				{
					/*
					if (counter >= 0 && counter < 4)
						textSFX->play(0);
					else
						text_Fin_SFX->play(0);
					*/
					if (counter == 0)
						textSFX[TEXT_START_SFX]->play(0);
					else if (counter < 4)
						textSFX[TEXT_MID_SFX]->play(0);
					else
						textSFX[TEXT_END_SFX]->play(0);
					text0[counter]->setVisible(true);
					counter++;
					timer = -1;	
					animation_flag = false;								
	
					if (counter >= 5)
					{
						counter = 0;
						state = PAN;
					}
				}
			}
			break;
		}
		case PAN:
		{
			if (!animation_flag)
			{
				timer = getMs();
				animation_flag = true;
			}
			else
			{
				if (getMs() - timer >= 2500)
				{
					yScene -= 0.5;
				}
				if (yScene <= yFinal)
				{
					yScene = yFinal;
					timer = -1;
					animation_flag = false;
					state = FINAL;
				}
			}
			break;
		}
		case FINAL:
		{
			break;
		}
	}

	sunAnimation.update();
	bmoAnimation.update();
	score->setXOffset((int)xScene);
    score->setYOffset((int)yScene);
	for (int i=0; i<text0_size; i++)
	{
		text0[i]->setXOffset((int)xScene);
		text0[i]->setYOffset((int)yScene);
	}
	for (int i=0; i<text1_size; i++)
    {
		text1[i]->setXOffset((int)xScene);
        text1[i]->setYOffset((int)yScene);
    }
	for (int i=0; i<text2_size; i++)
    {
		text2[i]->setXOffset((int)xScene);
        text2[i]->setYOffset((int)yScene);
    }
}
void WinState::draw()
{
	SDL_SetRenderTarget( renderTarget, preScaleTexture );
	//SDL_SetRenderDrawColor(renderTarget, 0xFF, 0, 0, 0xFF);
    SDL_RenderClear(renderTarget);
	//SDL_SetRenderDrawColor(renderTarget, 0, 0, 0, 0xFF);

	//---------------------

	for (int i=0; i<text0_size; i++)
		text0[i]->draw();
	for (int i=0; i<text1_size; i++)
        text1[i]->draw();
	for (int i=0; i<text2_size; i++)
        text2[i]->draw();

	score->draw(score_val);

	SDL_Rect cropRect = sunAnimation.getImageRect();
	SDL_Rect posRect = {xSun+(int)xScene, ySun+(int)yScene, 2*wSun, 2*hSun};
	SDL_RenderCopy(renderTarget, sunAnimation.getFrameTexture(), &cropRect, &posRect);

	cropRect = bmoAnimation.getImageRect();
    posRect.x = xBmo+(int)xScene;
	posRect.y = yBmo+(int)yScene;
	posRect.w = wBmo;
	posRect.h = hBmo;
    SDL_RenderCopy(renderTarget, bmoAnimation.getFrameTexture(), &cropRect, &posRect);

	//---------------------
	//SDL_SetRenderTarget( renderTarget, NULL );
    //SDL_Rect preScaleRect = {11, 12, 206*3, 152*3};
    //SDL_RenderCopy(renderTarget, preScaleTexture, NULL, &preScaleRect);


	//----------------------
	SDL_SetRenderTarget( renderTarget, NULL );

	int width_N = GamePanel::WINDOW_WIDTH/206;
	int height_N = GamePanel::WINDOW_HEIGHT/152;
	int scale = width_N;
	if (width_N > height_N)
	{
		scale = height_N;
	}

	int width_R = GamePanel::WINDOW_WIDTH - 206*scale;
	int height_R = GamePanel::WINDOW_HEIGHT - 152*scale;

	//SDL_Rect preScaleRect = {width_R/2, height_R/2, 206*width_N, 152*height_N};
	SDL_Rect preScaleRect = {width_R/2, height_R/2, 206*scale, 152*scale};
    SDL_RenderCopy(renderTarget, preScaleTexture, NULL, &preScaleRect);	
}
void WinState::keyPressed(int k)
{
	if (state != FINAL)
		return;
	switch(k)
    {
        case SDLK_RETURN:
        {
            gsm->setState(GameStateManager::TITLE_STATE);
            break;
        }
		case SDLK_RSHIFT:
        {
            *(GamePanel::isRunningControl) = false;
            break;
        }
    }
}
void WinState::keyReleased(int k)
{

}
