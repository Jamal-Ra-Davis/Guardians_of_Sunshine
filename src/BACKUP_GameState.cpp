#include <stdio.h>
#include "../headers/GameState.h"
#include "../headers/SDL_Helpers.h"
#include "../headers/GamePanel.h"
#include "../headers/TileMap.h"
#include "../headers/Slugger.h"

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
	player = NULL;
	gos_player = NULL;
	hud = NULL;
	bgm = NULL;
	coin = NULL;
	score = 0;
	trackPlayerY = false;

	for (int i=0; i<4; i++)
		triggerBoxes[i] = NULL;
	
	//init();
}
Level1State::~Level1State()
{
	if (tileMap)
		delete tileMap;
	tileMap = NULL;

	if (bgTileMap)
		delete bgTileMap;
	bgTileMap = NULL;

	if (bg)
		delete bg;
	bg = NULL;

	if (player)
		delete player;
	player = NULL;

	if (gos_player)
        delete gos_player;
    gos_player = NULL;

	if (hud)
		delete hud;
	hud = NULL;

	if (bgm)
		delete bgm;
	bgm = NULL;

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

	if (coin)
		delete coin;
	coin = NULL;



	for (int i=0; i<4; i++)
	{
		if (triggerBoxes[i])
			triggerBoxes[i];
		triggerBoxes[i] = NULL;
	}
	//Delete elements from lists
}

void Level1State::init()
{
	score = 0;
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
	
	player = new Player(tileMap, renderTarget);
	player->setPosition(30, 30);//75, 0
	player->setVector(0, 0);

	gos_player = new GoS_Player(tileMap, renderTarget);
    gos_player->setPosition(70, 105);//75, 0
    gos_player->setVector(0, 0);

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


	coin = new Coin(tileMap, renderTarget, 10);
	coin->setPosition(30, 90);
	populateCoins();

	triggerBoxes[0] = new TriggerBox(96, 105, 6, 60, false);
	triggerBoxes[1] = new TriggerBox(110, 105, 6, 60, false);
	triggerBoxes[2] = new TriggerBox(200, 154, 6, 60, false);
	triggerBoxes[3] = new TriggerBox(224, 154, 6, 60, false);

	trackPlayerY = false;
	playerYLock = 105;
/*
	populateEnemies();
	hud = new HUD(player, renderTarget, 0, 10);
*/
	bgm = new AudioPlayer("./Resources/Music/BMOGame_LevelLoop.ogg", true);
	//bgm = new AudioPlayer("./Resources/Music/BMOGame_Intro.mp3", true);
	bgm->play(AudioPlayer::LOOP);



	//Display Level 1 Screen?
}
void Level1State::update()
{
	//printf("Score: %d\n", score);
	//player->update();
	gos_player->update();
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

	//if (gos_player->intersects(coin) && !coin->isCollected())
	//{
	//	coin->collect();
	//}
	//printf("(X,Y) = %d, %d\n", (int)gos_player->getX(), (int)gos_player->getY());

	sun->update();
	slime->update();
	coin->update();
	if (gos_player->intersects(coin) && !coin->isCollected())
    {
        coin->collect();
    }
	if (coin->shouldRemove())
	{
		//Remove coin from list
		int butts = 0;
	}

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
	if (trackPlayerY)
	{
		tileMap->setPosition(GamePanel::WIDTH/2 - gos_player->getX(),
        	               GamePanel::HEIGHT/2 - gos_player->getY() + 12);
	}
	else
	{
		tileMap->setPosition(GamePanel::WIDTH/2 - gos_player->getX(),
                           GamePanel::HEIGHT/2 - playerYLock + 12);
	}
	double x = tileMap->getX();
    double y = tileMap->getY();
    double x1 = bgTileMap->getX() - x;
    double y1 = bgTileMap->getY() - y;
	bgTileMap->setPosition((int)(tileMap->getX()+0), (int)(tileMap->getY()-66));
/*
	bg->setPosition(tileMap->getX(), tileMap->getY());

	player->checkAttack(enemies);
	if (player->getY() > 400 || player->isDead())
    {
        *(GamePanel::isRunningControl) = false;
    }	


	std::list<Enemy*>::iterator iter;
    for (iter=enemies.begin(); iter != enemies.end(); )
    {
		Enemy *enemy = (*iter);
        enemy->update();
		if (enemy->isDead())
		{
			int xPos = enemy->getX();
			int yPos = enemy->getY();
			delete enemy;
			*iter = NULL;
			iter = enemies.erase(iter);
			explosions.push_back(new Explosion(renderTarget, xPos, yPos));
			
		}
		else
		{
			++iter;
		}
    }
	
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

	SDL_Texture *tempTexture = NULL;
	//tempTexture = SDL_CreateTexture(renderTarget, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, GamePanel::WIDTH, GamePanel::HEIGHT);
	tempTexture = SDL_CreateTexture(renderTarget, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 206, 116);
	//SDL_Rect scaleRect = {0, 0, (int)(GamePanel::WIDTH*3/*current.w*/), (int)(GamePanel::HEIGHT*3/*current.h*/)};
	SDL_Rect scaleRect = {11, 11+110, 3*206, 3*116};
	SDL_SetRenderTarget( renderTarget, tempTexture );


	//Draw Background
	//bg->draw();

	//Draw Tilemap
	bgTileMap->draw();
	sun->draw();
	slime->draw();
	coin->draw();
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
	for (int i=0; i<4; i++)
	{
		triggerBoxes[i]->draw(tileMap, renderTarget);
	}

	//player->draw();
	gos_player->draw();

	//printf("DEBUG: Player Drawn\n");
/*
	std::list<Enemy*>::iterator iter;
    for (iter=enemies.begin(); iter != enemies.end(); ++iter)
    {
        (*iter)->draw();
    }

	std::list<Explosion*>::iterator explosions_iter;
    for (explosions_iter=explosions.begin(); explosions_iter != explosions.end(); ++explosions_iter)
    {
        Explosion *explosion = *explosions_iter;
		explosion->setMapPosition((int)tileMap->getX(), (int)tileMap->getY());
        explosion->draw();
    }



	
	hud->draw();
*/

	SDL_SetRenderTarget( renderTarget, NULL );
	SDL_RenderCopy(renderTarget, tempTexture, NULL, &scaleRect);

	SDL_DestroyTexture(tempTexture);	
	tempTexture = NULL;
}
void Level1State::populateEnemies()
{
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
}
void Level1State::populateCoins()
{
	int locations[8][2] = {
							{1068 ,140},
							{1092 ,140},
							{1116 ,140},
							{1128 ,140},
							{1140 ,140},
							{1164, 140},
							{1188, 140},
							{1200, 140}
						};
	for (int i=0; i<8; i++)
	{
		Coin *c = new Coin(tileMap, renderTarget, 10);
		c->setPosition(locations[i][0], locations[i][1]);
		coins.push_back(c);
	}
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
			//x -= scroll_val;
			//x1 -= scroll_val;
            break;
        }
		case SDLK_RIGHT:
		{
			//player->setRight(true);
			gos_player->setRight(true);
			//x += scroll_val;
			//x1 += scroll_val;
			break;
		}
		case SDLK_UP:
		{
			//player->setUp(true);
			gos_player->setUp(true);
			//y -= scroll_val;
			//y1 -= scroll_val;
			break;
		}
		case SDLK_DOWN:
		{
			//player->setDown(true);
			gos_player->setDown(true);
			//y += scroll_val;
			//y1 += scroll_val;
			break;
		}
		case SDLK_SPACE:
		{
			//player->setJumping(true);
			gos_player->setJumping(true);
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
			break;
		}
		case SDLK_x:
		{
			//player->setFiring();
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
	SDL_Rect posRect = {0, 0, sprite_rects[0].w, sprite_rects[0].h};
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
