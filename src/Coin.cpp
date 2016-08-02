#include <stdio.h>
#include "../headers/Coin.h"
#include "../headers/SDL_Helpers.h"


Coin::Coin(TileMap *tm, SDL_Renderer *renderTarget_, int value_) : MapObject(tm, renderTarget_)
{
	value = value_;
	collected = false;
	should_remove = false;
	collectTimer = 0;
	disappearTime = 1000;

	width = 9;
	height = 17;
	cwidth = 9;
	cheight = 17;

	facingRight = true;

	//load sprites
	//numAnimations = NUM_ANIMATIONS;
    //numAnimations = 8;
    frameNumbers = NULL;
    frameNumbers = new int[NUM_ANIMATIONS];
    int tempNums[NUM_ANIMATIONS] = {1, 3, 3};
    for (int i=0; i<NUM_ANIMATIONS; i++)
        frameNumbers[i] = tempNums[i];

    sprite_rects = NULL;
    sprite_rects = new SDL_Rect*[NUM_ANIMATIONS];
    for (int i=0; i<NUM_ANIMATIONS; i++)
    {
        sprite_rects[i] = NULL;
        sprite_rects[i] = new SDL_Rect[frameNumbers[i]];
        //printf("DEBUG: frameNumbers[%d] = %d\n", i, frameNumbers[i]);
    }

	animationTexture = LoadTexture("./Resources/Sprites/GoS_CoinSprites.bmp", renderTarget);
    if (animationTexture)
        printf("DEBUG: Loaded player textures\n");
    for (int i=0; i<NUM_ANIMATIONS; i++)
    {
        for (int j=0; j<frameNumbers[i]; j++)
        {
        	sprite_rects[i][j].x = j*width;
            sprite_rects[i][j].y = i*height;
            sprite_rects[i][j].w = width;
            sprite_rects[i][j].h = height;
        }
    }

	currentAction = DEFAULT;
    animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
    animation.setDelay(-1);
    width = sprite_rects[currentAction][animation.getFrame()].w;

	coin_sound =  new AudioPlayer("./Resources/SFX/BMOGame_CoinCollect.wav", false);	
}

Coin::~Coin()
{
	delete frameNumbers;
    frameNumbers = NULL;
    for (int i=0; i<NUM_ANIMATIONS; i++)
    {
        delete [] sprite_rects[i];
        sprite_rects[i] = NULL;
    }
    delete [] sprite_rects;
    sprite_rects = NULL;

    SDL_DestroyTexture(animationTexture);
    animationTexture = NULL;

	if (coin_sound)
		delete coin_sound;
	coin_sound = NULL;
}

void Coin::collect()
{
	collected = true;
	currentAction = COLLECTED;
    animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
    animation.setDelay(100);
    width = sprite_rects[currentAction][animation.getFrame()].w;
	coin_sound->play(0);
}
void Coin::update()
{
	if (should_remove)
		return;
	if (currentAction == COLLECTED)
	{
		if (animation.hasPlayedOnce())
		{
			
    		currentAction = DISAPPEARING;
    		animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
    		animation.setDelay(100);
    		width = sprite_rects[currentAction][animation.getFrame()].w;
			collectTimer = getMs();
			
			//should_remove = true;
		}
	}
	if (currentAction == DISAPPEARING)
    {	
		/*
		if(animation.hasPlayedOnce())
			should_remove = true;
		*/
		if (getMs() - collectTimer >= disappearTime)
		{
			should_remove = true;
		}	
    }
	//printf("currentAction = %d\n", currentAction);
	//printf("animation.getFrame() = %d\n", animation.getFrame());
	//printf("");
	//if (!should_remove)
	animation.update();
}
void Coin::draw()
{

	if (should_remove)
		return;

	setMapPosition();
	SDL_Rect rect = getCollisionRect();
    //drawRect(rect);
    mapObjectDraw();
}
