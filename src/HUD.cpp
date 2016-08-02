#include "../headers/HUD.h"
#include "../headers/SDL_Helpers.h"

HUD::HUD(Player *player_, SDL_Renderer *renderTarget_, int x, int y)
{
	player = player_;
	renderTarget = renderTarget_;

	//Get textures
	hudImage = NULL;
	heartImage = NULL;

	hudImage = LoadTexture("./Resources/HUD/hud.bmp", renderTarget);	
	heartImage = LoadTexture("./Resources/HUD/heart.bmp", renderTarget);
	if (heartImage)
		printf("Heart image loaded\n");
	eHeartImage = LoadTexture("./Resources/HUD/heart_empty.bmp", renderTarget);

	SDL_QueryTexture(hudImage, NULL, NULL, &(hudImageRect.w), &(hudImageRect.h));
	hudImageRect.x = x;
	hudImageRect.y = y;
}
HUD::~HUD()
{
	SDL_DestroyTexture(hudImage);
	hudImage = NULL;
	SDL_DestroyTexture(heartImage);
	heartImage = NULL;
	SDL_DestroyTexture(eHeartImage);
    eHeartImage = NULL;
}
void HUD::draw()
{
	SDL_RenderCopy(renderTarget, hudImage, NULL, &hudImageRect);

	SDL_Rect heartImageRect;
	SDL_QueryTexture(heartImage, NULL, NULL, &(heartImageRect.w), &(heartImageRect.h));
	
	heartImageRect.y = hudImageRect.y + 7;
	for (int i=0; i<player->getMaxHealth(); i++)
	{
		heartImageRect.x = hudImageRect.x + 20 + i*(heartImageRect.w + 2);
		if (player->getHealth() >= i+1)
			SDL_RenderCopy(renderTarget, heartImage, NULL, &heartImageRect);	
		else
			SDL_RenderCopy(renderTarget, eHeartImage, NULL, &heartImageRect);
	}

	int fire = player->getFire();
	int rectWidth = player->getMaxHealth()*(heartImageRect.w + 2);
	SDL_Rect fireRect;

	fireRect.w = (int)(rectWidth*((1.0*player->getFire())/player->getMaxFire()));
	fireRect.h = heartImageRect.h;
	fireRect.y = hudImageRect.y + 28;
	fireRect.x = hudImageRect.x + 20;

	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderTarget, &r, &g, &b, &a);
	
	SDL_SetRenderDrawColor(renderTarget, 255, 100, 0, 255);
	SDL_RenderFillRect(renderTarget, &fireRect);
	SDL_SetRenderDrawColor(renderTarget, r, g, b, a);
}
