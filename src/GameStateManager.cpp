#include <stdio.h>
#include "../headers/GameStateManager.h"


GameStateManager::GameStateManager(SDL_Renderer *renderTarget_)
{
	renderTarget = renderTarget_;
	//currentState = MENU_STATE;
	currentState = TITLE_STATE;

	//currentState = WIN_STATE;
	score = 0;
	gameStates = new GameState*[NUM_STATES];
	for (int i=0; i<NUM_STATES; i++)
	{
		gameStates[i] = NULL;
	}
	loadState(currentState);
	gameStates[currentState]->init();
	//setState(currentState);
	printf("Loaded title state\n");
	//gameStates[0] = new MenuState(this, renderTarget);
	//gameStates[1] = new Level1State(this, renderTarget);
}

GameStateManager::~GameStateManager()
{
	for (int i=0; i<NUM_STATES; i++)
    {
        if (gameStates[i])
			delete gameStates[i];
    }
	if (gameStates)
		delete[] gameStates;
}

void GameStateManager::setState(int state)
{
	unloadState(currentState);
	currentState = state;
	loadState(currentState);
	gameStates[currentState]->init();
}
void GameStateManager::update()
{
	gameStates[currentState]->update();
}
void GameStateManager::draw()
{
	gameStates[currentState]->draw();
}
void GameStateManager::keyPressed(int k)
{
	gameStates[currentState]->keyPressed(k);
}
void GameStateManager::keyReleased(int k)
{
	gameStates[currentState]->keyReleased(k);
}

void GameStateManager::loadState(int state)
{
	switch (state)
	{
		case MENU_STATE:
		{
			if (!gameStates[state])
			{
				gameStates[state] = new MenuState(this, renderTarget);
			}
			break;
		}
		case TITLE_STATE:
        {
            if (!gameStates[state])
            {
                gameStates[state] = new TitleState(this, renderTarget);
            }
            break;
        }
		case LEVEL1_STATE:
		{
			if (!gameStates[state])
			{
				gameStates[state] = new Level1State(this, renderTarget);
			}
			break;
		}
		case WIN_STATE:
		{
			if (!gameStates[state])
			{
				gameStates[state] =  new WinState(this, renderTarget);
			}
			break;
		}
	}
}
void GameStateManager::unloadState(int state)
{
	if (gameStates[state])
	{
		delete gameStates[state];
		gameStates[state] = NULL;
	}
}
