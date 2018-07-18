#include <time.h>
#include <stdio.h>

#include "../headers/Player.h"
#include "../headers/SDL_Helpers.h"

Player::Player(TileMap *tm, SDL_Renderer *renderTarget_) : MapObject(tm, renderTarget_)
{
	left = false;
    right = false;
    up = false;
    down = false;
    jumping = false;
    falling = false;

	firing = false;
	scratching = false;
	gliding = false;
	flinching = false;
	dead = false;

	flinchTimer = 0;


	width = 30;
	height = 30;
	cwidth = 12;
	cheight = 20;
	
	moveSpeed = 0.3;
	maxSpeed = 1.6;
	stopSpeed = 0.4;
	fallSpeed = 0.15;
	maxFallSpeed = 4.0;
	jumpStart = -4.8;
	stopJumpSpeed = 0.3;

	facingRight = true;

	maxHealth = 5;
	health = maxHealth;

	maxFire = 2500;
	fire = maxFire;
	fireCost = 200;
	fireBallDamage = 5;
	//fireBalls = newList<FireBall>???

	scratchDamage = 8;
	scratchRange = 40;


	//load sprites
	numAnimations = 7;
	frameNumbers = NULL;
	frameNumbers = new int[numAnimations];
	int tempNums[7] = {2, 8, 1, 2, 4, 2, 5};
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

	animationTexture = LoadTexture("./Resources/Sprites/Player/playersprites.bmp", renderTarget);
	if (animationTexture)
		printf("DEBUG: Loaded player textures\n");
	for (int i=0; i<numAnimations; i++)
	{
		for (int j=0; j<frameNumbers[i]; j++)
		{
			if (i != 6)
			{
				sprite_rects[i][j].x = j*width;
				sprite_rects[i][j].y = i*height;
				sprite_rects[i][j].w = width;
				sprite_rects[i][j].h = height;
			}
			else
			{
				sprite_rects[i][j].x = j*width*2;
                sprite_rects[i][j].y = i*height;
                sprite_rects[i][j].w = width*2;
                sprite_rects[i][j].h = height;
			}
		}
	}

	currentAction = IDLE;
   	animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
  	animation.setDelay(400);
 	width = sprite_rects[currentAction][animation.getFrame()].w;


	//Audio loading
	player_sounds = NULL;
	player_sounds = new AudioPlayer*[SFX_NUM];	
	for (int i=0; i<SFX_NUM; i++)
	{
		player_sounds[i] = NULL;
	}
	player_sounds[JUMP_SFX] = new AudioPlayer("./Resources/SFX/jump.wav", false);
	player_sounds[SCRATCH_SFX] = new AudioPlayer("./Resources/SFX/scratch.wav", false);

	//printf("DEBUG: Completed Player constructor\n");	
}
Player::~Player()
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

	std::list<FireBall*>::iterator iter;
    for (iter=fireBalls.begin(); iter != fireBalls.end(); ++iter)
    {
		FireBall *fb = (*iter);
		delete fb;
        (*iter) = NULL;
    }

	for (int i=0; i<SFX_NUM; i++)
	{
		delete player_sounds[i];
		player_sounds[i] = NULL;
	}
	delete player_sounds;
	player_sounds = NULL;
}

void Player::getNextPosition()
{
	//movement
	if (left)
	{
		dx -= moveSpeed;
		if (dx < -maxSpeed)
		{
			dx = -maxSpeed; 
		}
	}
	else if (right)
	{
		dx += moveSpeed;
		if (dx > maxSpeed)
		{
			dx = maxSpeed;
		}
	}
	else
	{
		if (dx > 0)
		{
			dx -= stopSpeed;
			if (dx < 0)
				dx = 0;
		}
		else if (dx < 0)
		{
			dx += stopSpeed;
			if (dx > 0)
				dx = 0;
		}
	}


	//Cannot attack while moveing unless in air
	if ((currentAction == SCRATCHING || currentAction == FIREBALL) && !(jumping || falling))
	{
		dx = 0;
	}


	//Jumping
	if (jumping && !falling)
	{
		player_sounds[JUMP_SFX]->play(0);
		dy = jumpStart;
		falling = true;
	}

	//falling
	if (falling)
	{
		if (dy > 0 && gliding)
			dy += fallSpeed * 0.1;
		else
			dy += fallSpeed;

		if (dy > 0)
			jumping = false;
		if (dy < 0 && !jumping)
			dy += stopJumpSpeed;
		if (dy > maxFallSpeed)
			dy = maxFallSpeed;
	}

}
void Player::hit(int damage)
{
	if (flinching)
		return;
	health -= damage;
	if (health <= 0)
	{
		health = 0;
		dead = true;
	}
	flinching = true;
	flinchTimer = getMs();
}
void Player::update()
{
	//update position
	getNextPosition();
	checkTileMapCollision();
	setPosition(xtemp, ytemp);

	//check attack has stopped
	if (currentAction == SCRATCHING)
	{
		if (animation.hasPlayedOnce())
			scratching = false;
	}
	if (currentAction == FIREBALL)
    {
        if (animation.hasPlayedOnce())
            firing = false;
    }


	//fireball attack
	fire++;
	if (fire > maxFire)
		fire = maxFire;
	if (firing && currentAction != FIREBALL)
	{
		if (fire > fireCost)
		{
			fire -= fireCost;
			FireBall *fb = new FireBall(tileMap, renderTarget, facingRight);
			fb->setPosition(x, y);
			fireBalls.push_back(fb);
		}
	}

	//update fireballs
	std::list<FireBall*>::iterator iter;
    for (iter=fireBalls.begin(); iter != fireBalls.end(); /*++iter*/)
    {
		FireBall *fb = (*iter);
        fb->update();
		if (fb->shouldRemove())
		{
			delete fb;
			*iter = NULL;
			iter = fireBalls.erase(iter);
		}
		else
		{
			++iter;
		}
    }

	//check done flinching
	if (flinching)
	{
		double elapsed = getMs() - flinchTimer;
		if (elapsed > 1000)
			flinching = false;
	}


	//set animation
	if (scratching)
	{
		if (currentAction != SCRATCHING)
		{
			player_sounds[SCRATCH_SFX]->play(0);
			currentAction = SCRATCHING;	
			animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
			animation.setDelay(50);
			width = sprite_rects[currentAction][animation.getFrame()].w;
		}
	}
	else if (firing)
	{
		if (currentAction != FIREBALL)
		{
			currentAction = FIREBALL;
			animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
			animation.setDelay(100);
			width = sprite_rects[currentAction][animation.getFrame()].w;
		}
	}
	else if (dy > 0)
	{
		if (gliding)
		{
			if (currentAction != GLIDING)
			{
				currentAction = GLIDING;
				animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
				animation.setDelay(100);
				width = sprite_rects[currentAction][animation.getFrame()].w;
			}
		}
		else if (currentAction != FALLING)
		{
        	currentAction = FALLING;
        	animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
        	animation.setDelay(100);
        	width = sprite_rects[currentAction][animation.getFrame()].w;
		}
	}
	else if (dy < 0)
	{
		if (currentAction != JUMPING)
        {
            currentAction = JUMPING;
            animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
            animation.setDelay(-1);
            width = sprite_rects[currentAction][animation.getFrame()].w;
        }
	}
	else if (left || right)
	{
		if (currentAction != WALKING)
        {
            currentAction = WALKING;
            animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
            animation.setDelay(40);
            width = sprite_rects[currentAction][animation.getFrame()].w;
        }
	}
	else
	{
		if (currentAction != IDLE)
        {
            currentAction = IDLE;
            animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
            animation.setDelay(400);
            width = sprite_rects[currentAction][animation.getFrame()].w;
        }
	}
	
	animation.update();
	
	if (currentAction != SCRATCHING && currentAction != FIREBALL)
	{
		//set direction
		if (right)
			facingRight = true;
		else if (left)
			facingRight = false;
	}

}


void Player::draw()
{
	//printf("DEBUG: Entered player draw function\n");
	setMapPosition();

	std::list<FireBall*>::iterator iter;
	for (iter=fireBalls.begin(); iter != fireBalls.end(); ++iter)
	{
		(*iter)->draw();
	}

	
	if (flinching)
	{
		double elapsed = (getMs() - flinchTimer);
		if ((long)(elapsed / 100) % 2 == 0)
		{
			return;
		}
	}
	mapObjectDraw();

/*
	if (facingRight)
	{
		SDL_Rect posRect = {(int)(x + xmap - width/2), (int)(y + ymap - height/2), width, height};
		SDL_Rect cropRect = animation.getImageRect();
		SDL_RenderCopyEx(renderTarget, animation.getFrameTexture(), &cropRect, &posRect, 0, NULL, SDL_FLIP_NONE);
	}
	else
	{
		SDL_Rect posRect = {(int)(x + xmap - width/2), (int)(y + ymap - height/2), width, height};
		SDL_Rect cropRect = animation.getImageRect();
        SDL_RenderCopyEx(renderTarget, animation.getFrameTexture(), &cropRect, &posRect, 0, NULL, SDL_FLIP_HORIZONTAL);
	}
*/
	//printf("DEBUG: Exiting player draw function\n");
}
void Player::checkAttack(std::list<Enemy*> enemies)
{
	std::list<Enemy*>::iterator iter;
    for (iter=enemies.begin(); iter != enemies.end(); ++iter)
    {
		Enemy *enemy = *iter;
		if (scratching)
		{
			SDL_Rect eRect = enemy->getCollisionRect();
           	SDL_Rect aRect;
			if (facingRight)
			{
				aRect.x = (int)x;
				aRect.y = (int)(y - height/2);	
				aRect.w = scratchRange;
				aRect.h = height;
			}
			else
			{
				aRect.x = (int)(x - scratchRange);
                aRect.y = (int)(y - height/2);
                aRect.w = scratchRange;
                aRect.h = height;
			}

			if (rectOverlap(aRect, eRect))
           	{
            	enemy->hit(scratchDamage);
           	}
		}

		std::list<FireBall*>::iterator fbIter;
    	for (fbIter=fireBalls.begin(); fbIter != fireBalls.end(); ++fbIter)
    	{
			FireBall *fb = *fbIter;
			if (fb->intersects(enemy))
			{
				enemy->hit(fireBallDamage);
				fb->setHit();
				break;
			}			
    	}	

		//check enemy collison
		if (intersects(enemy))
		{
			if (!flinching)
			{
				hit(enemy->getDamage());
				if (!dead)
				{
					if (facingRight)
					{
						dx = -3;
						dy *= -1;
					}
					else
					{
						dx = 3;
						dy *= -1;
					}
				}
			}	
		}	
    }	
}









