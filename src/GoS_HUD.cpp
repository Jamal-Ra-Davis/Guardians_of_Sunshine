#include <stdio.h>
#include "../headers/GoS_HUD.h"



GoS_HUD::GoS_HUD(SDL_Renderer *renderTarget_, int x, int y)
{
	//player = player_;
    renderTarget = renderTarget_;
	//score = score_;	

	//Get textures
	hudBg = NULL;
    hudBg = LoadTexture("./Resources/HUD/GoS_HUD_BG.bmp", renderTarget);

    SDL_QueryTexture(hudBg, NULL, NULL, &(hudBgRect.w), &(hudBgRect.h));
    hudBgRect.x = x;
    hudBgRect.y = y;


	playerIcon = NULL;
    playerIcon = LoadTexture("./Resources/HUD/GoS_LivesIcon.bmp", renderTarget);

    SDL_QueryTexture(playerIcon, NULL, NULL, &(playerRect.w), &(playerRect.h));
    playerRect.x = hudBgRect.x + 32 + 2;
    playerRect.y = hudBgRect.y + hudBgRect.h/2 - playerRect.h/2;

	bombaIcon = NULL;
    bombaIcon = LoadTexture("./Resources/HUD/GoS_BombaIcon.bmp", renderTarget);

    SDL_QueryTexture(bombaIcon, NULL, NULL, &(bombaRect.w), &(bombaRect.h));
    bombaRect.x = hudBgRect.x + 64*2 + 32 + 3*2;
    bombaRect.y = hudBgRect.y + hudBgRect.h/2 - bombaRect.h/2;


	scoreTracker = NULL;
    scoreTracker = new Numbers(renderTarget, NUM_HUD_DIGITS, 5, 0, 0);
	scoreTracker->setX(hudBgRect.x + hudBgRect.w/2 - scoreTracker->getWidth()/2);
	scoreTracker->setY(hudBgRect.y + hudBgRect.h/2 - scoreTracker->getHeight()/2);
}
GoS_HUD::~GoS_HUD()
{
	SDL_DestroyTexture(hudBg);
    hudBg = NULL;

	SDL_DestroyTexture(playerIcon);
    playerIcon = NULL;

	SDL_DestroyTexture(bombaIcon);
    bombaIcon = NULL;
}
void GoS_HUD::draw(int score, int playerLives, int bombaCount)
{
	SDL_RenderCopy(renderTarget, hudBg, NULL, &hudBgRect);
	//scoreTracker->draw(*score);
	scoreTracker->draw(score);

	SDL_Rect tempRect = playerRect;
	tempRect.x = tempRect.x - 32/2 - playerRect.w/2;
	for (int i=0; i<playerLives; i++)
	{
		SDL_RenderCopy(renderTarget, playerIcon, NULL, &tempRect);
		tempRect.x += 16;
	}

	tempRect = bombaRect;
    tempRect.x = tempRect.x - 32/2 - bombaRect.w/2;
	for (int i=0; i<bombaCount; i++)
	{
		SDL_RenderCopy(renderTarget, bombaIcon, NULL, &tempRect);
		tempRect.x += 16;
	}
	
}
