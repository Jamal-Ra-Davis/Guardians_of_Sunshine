#include <stdio.h>
#include "../headers/GoS_StartMenu.h"
#include "../headers/SDL_Helpers.h"
#include "../headers/InputBuffer.h"

GoS_StartMenu::GoS_StartMenu(SDL_Renderer *renderTarget_, int x, int y)
{
    renderTarget = renderTarget_;

	//Get textures
	menuBg = NULL;
    menuBg = LoadTexture("./Resources/HUD/GoS_Start_Menu.bmp", renderTarget);

    SDL_QueryTexture(menuBg, NULL, NULL, &(menuBgRect.w), &(menuBgRect.h));
    menuBgRect.x = x;
    menuBgRect.y = y;

	comboIcon = NULL;
	comboIcon = LoadTexture("./Resources/HUD/GoS_Combo_Icons.bmp", renderTarget);
	SDL_QueryTexture(comboIcon, NULL, NULL, &(comboRect.w), &(comboRect.h));
	comboRect.w /= InputBuffer::NUM_KEYS;
	//comboRect.h /= InputBuffer::NUM_KEYS;
	comboRect.x = x + 10;
	comboRect.y = y + 30;

	visible = false;
	/*
	playerIcon = NULL;
    playerIcon = LoadTexture("./Resources/HUD/GoS_LivesIcon.bmp", renderTarget);

    SDL_QueryTexture(playerIcon, NULL, NULL, &(playerRect.w), &(playerRect.h));
    playerRect.x = menuBgRect.x + 32 + 2;
    playerRect.y = menuBgRect.y + menuBgRect.h/2 - playerRect.h/2;

	bombaIcon = NULL;
    bombaIcon = LoadTexture("./Resources/HUD/GoS_BombaIcon.bmp", renderTarget);

    SDL_QueryTexture(bombaIcon, NULL, NULL, &(bombaRect.w), &(bombaRect.h));
    bombaRect.x = menuBgRect.x + 64*2 + 32 + 3*2;
    bombaRect.y = menuBgRect.y + menuBgRect.h/2 - bombaRect.h/2;


	scoreTracker = NULL;
    scoreTracker = new Numbers(renderTarget, NUM_HUD_DIGITS, 5, 0, 0);
	scoreTracker->setX(menuBgRect.x + menuBgRect.w/2 - scoreTracker->getWidth()/2);
	scoreTracker->setY(menuBgRect.y + menuBgRect.h/2 - scoreTracker->getHeight()/2);
	*/
}
GoS_StartMenu::~GoS_StartMenu()
{
	SDL_DestroyTexture(menuBg);
    menuBg = NULL;




/*
	SDL_DestroyTexture(playerIcon);
    playerIcon = NULL;

	SDL_DestroyTexture(bombaIcon);
    bombaIcon = NULL;
*/
}
void GoS_StartMenu::update()
{

}
void GoS_StartMenu::draw()
{
	int x_offset = 15;
	int x_spacing = 1;

	if (!visible)
		return;
	SDL_RenderCopy(renderTarget, menuBg, NULL, &menuBgRect);


	SDL_Rect comboCrop = {0, 0, comboRect.w, comboRect.h};
	for (int i=0; i<COMBO_LENGTH; i++)
	{
		int combo_idx = InputBuffer::data[i];
		comboCrop.x = comboRect.w*combo_idx;
		comboRect.x = (comboRect.w + x_spacing)*i + x_offset;
		SDL_RenderCopy(renderTarget, comboIcon, &comboCrop, &comboRect);
	}
	
	/*
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
	*/
}
