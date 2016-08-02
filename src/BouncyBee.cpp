#include "../headers/BouncyBee.h"
#include "../headers/SDL_Helpers.h"

BouncyBee::BouncyBee(TileMap *tm, SDL_Renderer *renderTarget_) : Enemy(tm, renderTarget_)
{	
	dx = 0;
    dy = 0;

	targX = 860;
	targY = 154;

    maxSpeed = 1.8;
	moveSpeed = maxSpeed;
    maxFallSpeed = 0;
	fallSpeed = maxFallSpeed;

    width = 52;
    height = 40;
    cwidth = 52;
    cheight = 40;

    maxHealth = 1;
    health = maxHealth;
    damage = 1;

    dead = false;
    flinching = false;
	active = false;
	triggerState = false;

	triggerBox = NULL;

	//load sprites
    numAnimations = 2;
    frameNumbers = NULL;
    frameNumbers = new int[numAnimations];
    int tempNums[2] = {1, 2};
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

	animationTexture = LoadTexture("./Resources/Sprites/Enemies/GoS_BouncyBeeSprites.bmp", renderTarget);
    if (animationTexture)
        printf("DEBUG: Loaded bouncy bee textures\n");
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

	bee_sound = new AudioPlayer("./Resources/SFX/BMOGame_BouncyBeeIntro.wav", false);

    right = true;
    facingRight = right;
}
BouncyBee::~BouncyBee()
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

	if (bee_sound)
		delete bee_sound;
	bee_sound = NULL;
}
void BouncyBee::onTrigger()
{
	bee_sound->play(0, 6);	
} 
void BouncyBee::hit(int damageTaken)
{
	if (dead || flinching || !active)
        return;

    health -= damageTaken;
    if (health <= 0)
    {
        health = 0;
        dead = true;
    }
    flinching = true;
    flinchTimer = getMs();
}
bool BouncyBee::isReady()
{
	return (active && at_target);
}
void BouncyBee::checkBombaCollision(Bomba *bomba)
{
    if (!bomba)
        return;
    if (intersects(bomba))
    {
        bomba->setRemove(true);
        hit(bomba->getDamage());
    }
}    
void BouncyBee::update()
{
	if (triggerBox)
	{
		//printf("bouncyBee::triggerBox = %d\n", triggerBox);
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

	dx = 0;
	dy = 0;
	bool prev_at_target = at_target;
	at_target = false;
	double delta = targX - x;
	if ((int)targX - (int)x > 0)
	{
		facingRight = false;
		if (delta > maxSpeed)
		{
			dx = maxSpeed;
		}
		else
		{
			dx = delta;
		}
	}	
	else if ((int)targX - (int)x < 0)
	{
		facingRight = true;
		if (-1*delta > maxSpeed)
		{
			dx = -1*maxSpeed;
		}
		else
		{
			dx = delta;
		}
	}
	else
	{
		at_target = true;
	}	
	if (!prev_at_target && at_target)
	{
		bee_sound->stop(6);
	}

	x += dx;
	y += dy;

	if (dx || dy)
	{
		if (currentAction != FLYING)
		{
			currentAction = FLYING;
            animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
            animation.setDelay(300);
            width = sprite_rects[currentAction][animation.getFrame()].w;
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
	triggerState = true;
}
void BouncyBee::draw()
{
	setMapPosition();

    if (flinching)
    {
        double elapsed = (getMs() - flinchTimer);
        if ((long)(elapsed / 100) % 2 == 0)
        {
            return;
        }
    }
    SDL_Rect rect = getCollisionRect();
    //drawRect(rect);
    mapObjectDraw();
}
