#include <stdio.h>
#include "../headers/GoS_StartMenu.h"
#include "../headers/SDL_Helpers.h"
#include "../headers/InputBuffer.h"
#include "../headers/GoS_Player.h"

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
	x_offset = 15;
	y_offset = 27;
	x_spacing = 1;	

	//comboRect.x = x + 10;
	comboRect.y = y + y_offset;


	visible = false;
	allow_visible = true;
	combo_text = NULL;
	controls_jump = NULL;
	controls_walk = NULL;
	controls_kick = NULL;
	controls_bomba = NULL;


	combo_text = new Letters(renderTarget, "COMBO MOVE", 3, 15, 17);

	controls_jump = new Letters(renderTarget, "SPACE", 3, 15, 20+37);
	controls_walk = new Letters(renderTarget, "ARROWS", 3, 15, 20+67);
	controls_kick = new Letters(renderTarget, "Z", 3, 120, 20+37);
	controls_bomba = new Letters(renderTarget, "X", 3, 120, 20+67);



	width = 25;
    height = 27;

	player_texture = NULL;
	printf("Trying to load player textures\n");
	player_texture = LoadTexture("./Resources/Sprites/Player/GoS_PlayerSprites.bmp", renderTarget);
	printf("Loaded player textures\n");

	numAnimations = 13;
    frameNumbers = NULL;
    frameNumbers = new int[numAnimations];
    int tempNums[13] = {3, 1, 2, 1, 3, 1, 1, /*2*/1, 9, 3, 6, 1, 2};
    for (int i=0; i<numAnimations; i++)
        frameNumbers[i] = tempNums[i];

	sprite_rects = NULL;
    sprite_rects = new SDL_Rect*[numAnimations];
    for (int i=0; i<numAnimations; i++)
    {
        sprite_rects[i] = NULL;
        sprite_rects[i] = new SDL_Rect[frameNumbers[i]];
        //printf("DEBUG: frameNumbers[%d] = %d\n", i, frameNumbers[i]);
    }


	for (int i=0; i<numAnimations; i++)
    {
        for (int j=0; j<frameNumbers[i]; j++)
        {
            if (i != 7)
            {
                sprite_rects[i][j].x = j*width;
                sprite_rects[i][j].y = i*height;
                sprite_rects[i][j].w = width;
                sprite_rects[i][j].h = height;
            }
            else
            {
                sprite_rects[i][j].x = (j+1)*width;
                sprite_rects[i][j].y = i*height;
                sprite_rects[i][j].w = width;
                sprite_rects[i][j].h = height;
            }
        }
    }
	currentAction_jump = GoS_Player::IDLE;
    jump_anim.setFrames(player_texture, sprite_rects[currentAction_jump], frameNumbers[currentAction_jump], true);
    jump_anim.setDelay(750);
    width = sprite_rects[currentAction_jump][jump_anim.getFrame()].w;

	currentAction_walk = GoS_Player::WALKING;
    walk_anim.setFrames(player_texture, sprite_rects[currentAction_walk], frameNumbers[currentAction_walk]);
    walk_anim.setDelay(300);
    width = sprite_rects[currentAction_walk][walk_anim.getFrame()].w;

	currentAction_kick = GoS_Player::IDLE;
    kick_anim.setFrames(player_texture, sprite_rects[currentAction_kick], frameNumbers[currentAction_kick], true);
    kick_anim.setDelay(750);
    width = sprite_rects[currentAction_kick][kick_anim.getFrame()].w;

	currentAction_bomba = GoS_Player::IDLE;
    bomba_anim.setFrames(player_texture, sprite_rects[currentAction_bomba], frameNumbers[currentAction_bomba], true);
    bomba_anim.setDelay(750);
    width = sprite_rects[currentAction_bomba][bomba_anim.getFrame()].w;

}
GoS_StartMenu::~GoS_StartMenu()
{
	if (menuBg)
		SDL_DestroyTexture(menuBg);
    menuBg = NULL;

	if (comboIcon)
		SDL_DestroyTexture(comboIcon);
	comboIcon = NULL;

	if (combo_text)
		delete combo_text;
	combo_text = NULL;


	if (controls_jump)
		delete controls_jump;
	controls_jump = NULL;

	if (controls_walk)
        delete controls_walk;
    controls_walk = NULL;

	if (controls_kick)
        delete controls_kick;
    controls_kick = NULL;

	if (controls_bomba)
        delete controls_bomba;
    controls_bomba = NULL;


	if (player_texture)
        SDL_DestroyTexture(player_texture);
    player_texture = NULL;

	if (frameNumbers)
		delete frameNumbers;
	frameNumbers = NULL;


	for (int i=0; i<numAnimations; i++)
    {
        delete [] sprite_rects[i];
        sprite_rects[i] = NULL;
    }
    delete [] sprite_rects;
    sprite_rects = NULL;

/*
	SDL_DestroyTexture(playerIcon);
    playerIcon = NULL;

	SDL_DestroyTexture(bombaIcon);
    bombaIcon = NULL;
*/
}
void GoS_StartMenu::update()
{
	update_jump();
	update_walk();
	update_kick();
	update_bomba();
}
void GoS_StartMenu::update_jump()
{
	if (currentAction_jump == GoS_Player::IDLE && jump_anim.hasPlayedOnce())
	{
		currentAction_jump = GoS_Player::JUMPING_START;
        jump_anim.setFrames(player_texture, sprite_rects[currentAction_jump], frameNumbers[currentAction_jump], true);
        jump_anim.setDelay(75);
        width = sprite_rects[currentAction_jump][jump_anim.getFrame()].w;
	}
	if (currentAction_jump == GoS_Player::JUMPING_START && jump_anim.hasPlayedOnce())
	{
		currentAction_jump = GoS_Player::JUMPING_HOLD;
        jump_anim.setFrames(player_texture, sprite_rects[currentAction_jump], frameNumbers[currentAction_jump], true);
		jump_anim.setDelay(400);
        width = sprite_rects[currentAction_jump][jump_anim.getFrame()].w;
	}
	if (currentAction_jump == GoS_Player::JUMPING_HOLD && jump_anim.hasPlayedOnce())
	{
		currentAction_jump = GoS_Player::JUMPING_LAND;
        jump_anim.setFrames(player_texture, sprite_rects[currentAction_jump], frameNumbers[currentAction_jump], true);
        jump_anim.setDelay(100);
        width = sprite_rects[currentAction_jump][jump_anim.getFrame()].w;
	}
	if (currentAction_jump == GoS_Player::JUMPING_LAND && jump_anim.hasPlayedOnce())
	{
		currentAction_jump = GoS_Player::LANDING;
        jump_anim.setFrames(player_texture, sprite_rects[currentAction_jump], frameNumbers[currentAction_jump], true);
        jump_anim.setDelay(75);
        width = sprite_rects[currentAction_jump][jump_anim.getFrame()].w;
	}
	if (currentAction_jump == GoS_Player::LANDING && jump_anim.hasPlayedOnce())
    {
        currentAction_jump = GoS_Player::IDLE;
        jump_anim.setFrames(player_texture, sprite_rects[currentAction_jump], frameNumbers[currentAction_jump], true);
        jump_anim.setDelay(750);
        width = sprite_rects[currentAction_jump][jump_anim.getFrame()].w;
    }
	
	jump_anim.update();
}
void GoS_StartMenu::update_walk()
{
	walk_anim.update();
}
void GoS_StartMenu::update_kick()
{
	if (currentAction_kick == GoS_Player::IDLE && kick_anim.hasPlayedOnce())
    {
        currentAction_kick = GoS_Player::KICKING;
        kick_anim.setFrames(player_texture, sprite_rects[currentAction_kick], frameNumbers[currentAction_kick], true);
        kick_anim.setDelay(65);
        width = sprite_rects[currentAction_kick][kick_anim.getFrame()].w;
    }
    if (currentAction_kick == GoS_Player::KICKING && kick_anim.hasPlayedOnce())
    {
        currentAction_kick = GoS_Player::IDLE;
        kick_anim.setFrames(player_texture, sprite_rects[currentAction_kick], frameNumbers[currentAction_kick], true);
        kick_anim.setDelay(750);
        width = sprite_rects[currentAction_kick][kick_anim.getFrame()].w;
    }

    kick_anim.update();
}
void GoS_StartMenu::update_bomba()
{
	if (currentAction_bomba == GoS_Player::IDLE && bomba_anim.hasPlayedOnce())
    {
        currentAction_bomba = GoS_Player::BOMBA_START;
        bomba_anim.setFrames(player_texture, sprite_rects[currentAction_bomba], frameNumbers[currentAction_bomba], true);
        bomba_anim.setDelay(50);
        width = sprite_rects[currentAction_bomba][bomba_anim.getFrame()].w;
    }
	if (currentAction_bomba == GoS_Player::BOMBA_START && bomba_anim.hasPlayedOnce())
    {
        currentAction_bomba = GoS_Player::BOMBA_THROW;
        bomba_anim.setFrames(player_texture, sprite_rects[currentAction_bomba], frameNumbers[currentAction_bomba], true);
        bomba_anim.setDelay(50);
        width = sprite_rects[currentAction_bomba][bomba_anim.getFrame()].w;
    }
    if (currentAction_bomba == GoS_Player::BOMBA_THROW && bomba_anim.hasPlayedOnce())
    {
        currentAction_bomba = GoS_Player::IDLE;
        bomba_anim.setFrames(player_texture, sprite_rects[currentAction_bomba], frameNumbers[currentAction_bomba], true);
        bomba_anim.setDelay(750);
        width = sprite_rects[currentAction_bomba][bomba_anim.getFrame()].w;
    }

    bomba_anim.update();
}
void GoS_StartMenu::draw()
{
	//int x_offset = 15;
	//int x_spacing = 1;

	if ((!visible || !allow_visible))
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

	combo_text->draw();
	controls_jump->draw();
    controls_walk->draw();
    controls_kick->draw();
    controls_bomba->draw();	

	int y_offset = 10;

	SDL_Rect jumpRect = {controls_jump->getX() + controls_jump->getWidth() + 17, controls_jump->getY() - y_offset, width, height};
	SDL_Rect jumpCrop = jump_anim.getImageRect();
	SDL_RenderCopy(renderTarget, player_texture, &jumpCrop, &jumpRect);	


	SDL_Rect walkRect = {controls_walk->getX() + controls_walk->getWidth() + 8, controls_walk->getY() - y_offset, width, height};
    SDL_Rect walkCrop = walk_anim.getImageRect();
    SDL_RenderCopy(renderTarget, player_texture, &walkCrop, &walkRect);


	SDL_Rect kickRect = {controls_kick->getX() + controls_kick->getWidth() + 17, controls_kick->getY() - y_offset, width, height};
    SDL_Rect kickCrop = kick_anim.getImageRect();
    SDL_RenderCopy(renderTarget, player_texture, &kickCrop, &kickRect);

	SDL_Rect bombaRect = {controls_bomba->getX() + controls_bomba->getWidth() + 17, controls_bomba->getY() - y_offset, width, height};
    SDL_Rect bombaCrop = bomba_anim.getImageRect();
    SDL_RenderCopy(renderTarget, player_texture, &bombaCrop, &bombaRect);

	//printf("Jump-x: %d, walk-x: %d\n", controls_jump->getX() + controls_jump->getWidth() + 10, controls_walk->getX() + controls_walk->getWidth() + 10);

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
