#include "../headers/HoneyBunny.h"
#include "../headers/SDL_Helpers.h"


HoneyBunny::HoneyBunny(TileMap *tm, SDL_Renderer *renderTarget_) : Enemy(tm, renderTarget_)
{
	triggerState = false;
	dx = 0;
	dy = 0;

	maxSpeed = 0;
    moveSpeed = maxSpeed;
    maxFallSpeed = 0;
    fallSpeed = maxFallSpeed;

	width = 45;
    height = 68;
    cwidth = 30;
    cheight = 68;

	poof_x = 0;//cwidth/2;
	poof_y = height/6;//2*cheight/3;


	face_rect.x = 0 - 4;
	face_rect.y = height/3 + 4;
	face_rect.w = width/2;
	face_rect.h = height/3 + 6;

	maxHealth = 10;
    health = maxHealth;
    damage = 1;

	dead = false;
    flinching = false;
    active = false;
	bunny_visible = true;
	slime_visible = true;
	spawned = false;
	face_collision = false;

    triggerBox = NULL;


	//load sprites
    numAnimations = 5;
    frameNumbers = NULL;
    frameNumbers = new int[numAnimations];
    int tempNums[5] = {1, 22, 1, 3, 1};
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

	animationTexture = LoadTexture("./Resources/Sprites/Enemies/GoS_HoneyBunnySprites.bmp", renderTarget);
    if (animationTexture)
        printf("DEBUG: Loaded honey bunny textures\n");
    for (int i=0; i<numAnimations; i++)
    {
        for (int j=0; j<frameNumbers[i]; j++)
        {
			if (i == 1)
			{
				int i_ = j/11 + 1;
				int j_ = j % 11;
            	sprite_rects[i][j].x = j_*width;
            	sprite_rects[i][j].y = i_*height;
            	sprite_rects[i][j].w = width;
            	sprite_rects[i][j].h = height;
			}
			else if (i >= 2)
			{
				//printf("DEBUG: i-1=%d, j=%d, frameNumbers[%d-1] = %d\n", i-1, j, i, frameNumbers[i-1]);
				sprite_rects[i/*-1*/][j].x = j*width;
                sprite_rects[i/*-1*/][j].y = (i+1)*height;
                sprite_rects[i/*-1*/][j].w = width;
                sprite_rects[i/*-1*/][j].h = height;
			}
			else
			{
				sprite_rects[i][j].x = j*width;
                sprite_rects[i][j].y = i*height;
                sprite_rects[i][j].w = width;
                sprite_rects[i][j].h = height;
			}
        }
    }
		
	currentAction = SPAWNING;
    animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
    animation.setDelay(100);
    width = sprite_rects[currentAction][animation.getFrame()].w;


	slimeWidth = 45;
    slimeHeight = 68;
    slimeCWidth = 45;
    slimeCHeight = 68;

	numSlimeAnimations = 3; //7 across
   	slimeAnimationTexture = NULL;
    slimeSprite_rects = NULL;
    slimeFrameNumbers = NULL;

    slimeFrameNumbers = new int[numSlimeAnimations];
    int tempNums2[3] = {1, 7, 4};
    for (int i=0; i<numSlimeAnimations; i++)
        slimeFrameNumbers[i] = tempNums2[i];

    slimeSprite_rects = new SDL_Rect*[numSlimeAnimations];
    for (int i=0; i<numSlimeAnimations; i++)
    {
        slimeSprite_rects[i] = NULL;
        slimeSprite_rects[i] = new SDL_Rect[slimeFrameNumbers[i]];
        //printf("DEBUG: slimeFrameNumbers[%d] = %d\n", i, slimeFrameNumbers[i]);
    }

	slimeAnimationTexture = LoadTexture("./Resources/Sprites/Enemies/GoS_SlimeDripSprites.bmp", renderTarget);
    if (animationTexture)
        printf("DEBUG: Loaded slime drip textures\n");
	for (int i=0; i<numSlimeAnimations; i++)
    {
        for (int j=0; j<slimeFrameNumbers[i]; j++)
        {
          	slimeSprite_rects[i][j].x = j*slimeWidth;
            slimeSprite_rects[i][j].y = i*slimeHeight;
            slimeSprite_rects[i][j].w = slimeWidth;
            slimeSprite_rects[i][j].h = slimeHeight;
        }
    }	

	slimeCurrentAction = SLIME_TEST;
    slimeAnimation.setFrames(slimeAnimationTexture, slimeSprite_rects[slimeCurrentAction], slimeFrameNumbers[slimeCurrentAction]);
    slimeAnimation.setDelay(-1);
    slimeWidth = sprite_rects[slimeCurrentAction][slimeAnimation.getFrame()].w;


	bunny_sound = new AudioPlayer("./Resources/SFX/BMOGame_HoneyBunnyIntro.wav", false);


    right = true;
    facingRight = right;

	bunny_visible = false;
	slime_visible = true;
}
HoneyBunny::~HoneyBunny()
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


	delete slimeFrameNumbers;
	slimeFrameNumbers = NULL;
	for (int i=0; i<numSlimeAnimations; i++)
	{
		delete [] slimeSprite_rects[i];
		slimeSprite_rects[i] = NULL;
	}
	delete [] slimeSprite_rects;
	slimeSprite_rects = NULL;
	SDL_DestroyTexture(slimeAnimationTexture);
	slimeAnimationTexture = NULL;	

	if (bunny_sound)
		delete bunny_sound;
	bunny_sound = NULL;
}
void HoneyBunny::setAnimation(int action, int delay, bool playsOnce, int loopNum)
{
	currentAction = action;
    animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], playsOnce, loopNum);
    animation.setDelay(delay);
    width = sprite_rects[currentAction][animation.getFrame()].w;
}
void HoneyBunny::onTrigger()
{
	//setAnimation(SPAWNING, 100, true);


	setAnimation(TEST, -1, true);
	slime_visible = true;	
	slimeCurrentAction = START;
    slimeAnimation.setFrames(slimeAnimationTexture, slimeSprite_rects[slimeCurrentAction], slimeFrameNumbers[slimeCurrentAction], true);
    slimeAnimation.setDelay(66);
    slimeWidth = sprite_rects[slimeCurrentAction][slimeAnimation.getFrame()].w;
	bunny_sound->play(-1, 6);
	/*
	currentAction = SPAWNING;
    animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
    animation.setDelay(100);
    width = sprite_rects[currentAction][animation.getFrame()].w;
	*/
}
void HoneyBunny::hit(int damageTaken)
{
	//printf("Honey Bunny hit function\n");
	if (dead || flinching || !active || !spawned)
        return;
	//printf("Made it past gating bools\n");
    health -= damageTaken;
    if (health <= 0)
    {
        health = 0;
        dead = true;
    }
    flinching = true;
    flinchTimer = getMs();
}
bool HoneyBunny::isReady()
{
	return (active && spawned);
}
void HoneyBunny::checkBombaCollision(Bomba *bomba)
{
	face_collision = false;
	if (!bomba)
		return;
	SDL_Rect bomba_rect = bomba->getCollisionRect();
	SDL_Rect tempFace_rect = face_rect;
	tempFace_rect.x += x - cwidth/2;
	tempFace_rect.y += y - cheight/2; 
	if (rectOverlap(tempFace_rect, bomba_rect))
	{
		face_collision = true;
	}
	if (intersects(bomba))
	{
		bomba->setRemove(true);
		hit(bomba->getDamage());
		
	}
	

}
void HoneyBunny::getPoofPosition(double *x_, double *y_)
{
	*x_ = x + poof_x;
	*y_ = y + poof_y;
}
void HoneyBunny::update()
{
/*
	active = true;
	//printf("Slime frame = %d\n", slimeAnimation.getFrame());
	slimeAnimation.update();
	return;
*/
	if (flinching)
	{
		if (getMs() - flinchTimer > 1000)
			flinching = false;
	}

	if (triggerBox)
    {
        if (triggerBox->wasTripped())
		{
            active = true;
		}
    }
	if (!active)
		return;

	if (!triggerState)
	{
		onTrigger();
	}



	if (slimeCurrentAction == START && slimeAnimation.hasPlayedOnce())
	{
		setAnimation(SPAWNING, 100, true);
		bunny_visible = true;

		slimeCurrentAction = LOOP;
    	slimeAnimation.setFrames(slimeAnimationTexture, slimeSprite_rects[slimeCurrentAction], slimeFrameNumbers[slimeCurrentAction]);
    	slimeAnimation.setDelay(66);
    	slimeWidth = sprite_rects[slimeCurrentAction][slimeAnimation.getFrame()].w;
	}

	//printf("Spawned: %d\n", spawned);
	if (currentAction == SPAWNING && animation.hasPlayedOnce())
	{
		setAnimation(IDLE, 2000, true);
		slime_visible = false;
		spawned = true;
		bunny_sound->stop(6);
	}

	if (spawned && (currentAction != IDLE && currentAction != MOVE_ARMS))
	{
		setAnimation(IDLE, 2000, true);
	}
	if (currentAction == IDLE && animation.hasPlayedOnce())
	{
		setAnimation(MOVE_ARMS, 100, true);
	}
	if (currentAction == MOVE_ARMS && animation.hasPlayedOnce())
    {
        setAnimation(IDLE, 3000, true);
    }
	
	if (spawned && face_collision)
	{
		setAnimation(HIT, -1);
	}

	animation.update();
	slimeAnimation.update();
	triggerState = true;
	face_collision = false;
}
void HoneyBunny::draw()
{
	//if (!bunny_visible)
	//	return;

	if (!active)
		return;

	setMapPosition();

	if (bunny_visible)
	{
    	if (flinching)
    	{
        	double elapsed = (getMs() - flinchTimer);
        	if ((long)(elapsed / 100) % 2 == 0)
        	{
            	return;
        	}
    	}
    	SDL_Rect rect = getCollisionRect();
		SDL_Rect tempFace_rect = face_rect;
    	tempFace_rect.x += x - cwidth/2;
    	tempFace_rect.y += y - cheight/2;
    	//drawRect(tempFace_rect);
		//drawRect(rect);
    	mapObjectDraw();
	}

	if (slime_visible)
	{
		SDL_Rect posRect = {(int)(x + xmap - slimeWidth/2), (int)(y + ymap - slimeHeight/2), slimeWidth, slimeHeight};
    	SDL_Rect cropRect = slimeAnimation.getImageRect();
    	SDL_RenderCopyEx(renderTarget, slimeAnimation.getFrameTexture(), &cropRect, &posRect, 0, NULL, SDL_FLIP_NONE);
	}
}
