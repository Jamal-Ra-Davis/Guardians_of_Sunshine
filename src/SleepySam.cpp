#include <stdio.h>
#include "../headers/SleepySam.h"
#include "../headers/SDL_Helpers.h"


SleepySam::SleepySam(TileMap *tm, SDL_Renderer *renderTarget_) : Enemy(tm, renderTarget_)
{
	dx = 0;
	dy = 0;

	maxSpeed = 0;
    moveSpeed = maxSpeed;
    maxFallSpeed = 0;
    fallSpeed = maxFallSpeed;

	active = false;
	visible = true;
	eat_player = false;
	eat_start = false;
	playedPoof = false;
	finished_eating = false;
	animationTimer = -1;
	animation_flag = false;

	comboMove = NULL;

	width = 51;
    height = 23;
    cwidth = 51;
    cheight = 23;	

	poof_x = 9;
	poof_y = 0;


	maxHealth = 100;
    health = maxHealth;
    damage = 1;

    dead = false;
    flinching = false;
    triggerBox = NULL;

	//load sprites
    numAnimations = 5;
    frameNumbers = NULL;
    frameNumbers = new int[numAnimations];
    int tempNums[5] = {1, 1, 11, 4, 5};
    for (int i=0; i<numAnimations; i++)
        frameNumbers[i] = tempNums[i];

    sprite_rects = NULL;
    sprite_rects = new SDL_Rect*[numAnimations];
    for (int i=0; i<numAnimations; i++)
    {
        sprite_rects[i] = NULL;
        sprite_rects[i] = new SDL_Rect[frameNumbers[i]];
        printf("DEBUG: frameNumbers[%d] = %d\n", i, frameNumbers[i]);
    }

	
	animationTexture = LoadTexture("./Resources/Sprites/Enemies/GoS_SleepySamSprites.bmp", renderTarget);
    if (animationTexture)
        printf("DEBUG: Loaded sleepy sam textures\n");
    for (int i=0; i<numAnimations; i++)
    {
        for (int j=0; j<frameNumbers[i]; j++)
        {
        	sprite_rects[i][j].x = j*width;
            sprite_rects[i][j].y = i*height;
            sprite_rects[i][j].w = width;
            sprite_rects[i][j].h = height;
        }
    }

	currentAction = IDLE;
    animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
    animation.setDelay(-1);
    width = sprite_rects[currentAction][animation.getFrame()].w;

	
	//Audio loading
    frog_sounds = NULL;
    frog_sounds = new AudioPlayer*[SFX_NUM];
    for (int i=0; i<SFX_NUM; i++)
    {
        frog_sounds[i] = NULL;
    }
    frog_sounds[LICK_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_SaltySam_Lick.wav", false);
	frog_sounds[SWALLOW_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_SaltySam_Swallow.wav", false);
	frog_sounds[POOP_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_SaltySam_Poop.wav", false);
	frog_sounds[POOP_FALL_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_SaltySam_PoopGround.wav", false);


	
	right = true;
    facingRight = right;
}
SleepySam::~SleepySam()
{
	delete frameNumbers;
    frameNumbers = NULL;
    for (int i=0; i<numAnimations; i++)
    {
        delete [] sprite_rects[i];
        sprite_rects[i] = NULL;
    }
    delete [] sprite_rects;
    sprite_rects = NULL;

    SDL_DestroyTexture(animationTexture);
    animationTexture = NULL;


	for (int i=0; i<SFX_NUM; i++)
    {
        delete frog_sounds[i];
        frog_sounds[i] = NULL;
    }
    delete frog_sounds;
    frog_sounds = NULL;
}
bool SleepySam::isReady()
{
	if (triggerBox)
		return triggerBox->wasTripped();
	else
		return false;
}
void SleepySam::getPoofPosition(double *x_, double *y_)
{
    *x_ = x + poof_x;
    *y_ = y + poof_y;
}
void SleepySam::reset()
{
	active = false;
    visible = true;
    eat_player = false;
    eat_start = false;
    playedPoof = false;
    finished_eating = false;
    animationTimer = -1;
	animation_flag = false;

	currentAction = IDLE;
    animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
    animation.setDelay(-1);
    width = sprite_rects[currentAction][animation.getFrame()].w;
}
void SleepySam::update()
{
	//active = true;
	if (triggerBox)
    {
        if (triggerBox->wasTripped())
        {
            active = true;
        }
    }
    if (!active || dead)
        return;

	if (eat_player)
	{
		if (!eat_start)
		{
			currentAction = EATING;
            animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
            animation.setDelay(60);
            width = sprite_rects[currentAction][animation.getFrame()].w;
			eat_start = true;

			frog_sounds[LICK_SFX]->play(0);
		}
		else
		{
			if (currentAction == EATING)
			{
				if (animation.hasPlayedOnce())
				{
					currentAction = IDLE;
            		animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
            		animation.setDelay(1000);
            		width = sprite_rects[currentAction][animation.getFrame()].w;
				}
				if (animation.getFrame() == 3)
				{
					if (comboMove)
					{
						comboMove->setVisible(false);
					}
				}
				if (animation.getFrame() == 7 && animation.frameChanged())
				{
					frog_sounds[SWALLOW_SFX]->play(0);
				}
			}
			else if (currentAction == IDLE)
			{
				if (animation.hasPlayedOnce())
                {
                    currentAction = POOPING;
                    animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
                    animation.setDelay(100);
                    width = sprite_rects[currentAction][animation.getFrame()].w;
					frog_sounds[POOP_SFX]->play(0);
                }
			}
			else if (currentAction == POOPING)
			{
			/*
				if (animation.hasPlayedOnce())
				{
					currentAction = POOP_FALL;
                    animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
                    animation.setDelay(100);
                    width = sprite_rects[currentAction][animation.getFrame()].w;
                    frog_sounds[POOP_FALL_SFX]->play(0);
				}
			*/
				if (animation.hasPlayedOnce() && !animation_flag)
				{
					animation_flag = true;
					animationTimer = getMs();
				}
				if (animation_flag && ((getMs() - animationTimer) >= 500))
                {
					animation_flag = false;			
	
					currentAction = POOP_FALL;
                    animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
                    animation.setDelay(75);
                    width = sprite_rects[currentAction][animation.getFrame()].w;
                    frog_sounds[POOP_FALL_SFX]->play(0);
                }
				/*
				if (animation.getFrame() == 4 && animation.frameChanged())
				{
					frog_sounds[POOP_FALL_SFX]->play(0);
				}
				if (animation.hasPlayedOnce() && animationTimer < 0)
				{
					animationTimer = getMs();	
				}
				if (animationTimer >= 0 && getMs() - animationTimer >= 1500)
				{
					finished_eating = true;	
				}
				*/
			}
			else if (currentAction == POOP_FALL)
			{
				if (animation.hasPlayedOnce() && !animation_flag)
                {
                    animationTimer = getMs();   
					animation_flag = true;
                }
				if (animation_flag && ((getMs() - animationTimer) >= 1500))
                {
                    finished_eating = true; 
                }
			}
		}	
	}
	else
	{
		if (currentAction != IDLE)
		{
			currentAction = IDLE;
    		animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
    		animation.setDelay(-1);
    		width = sprite_rects[currentAction][animation.getFrame()].w;
		}
	}

	animation.update();
}
void SleepySam::draw()
{
	if (!visible)
		return;

	setMapPosition();
	mapObjectDraw();
}
