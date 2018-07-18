#include <time.h>
#include <stdio.h>

#include "../headers/GoS_Player.h"
#include "../headers/SDL_Helpers.h"

GoS_Player::GoS_Player(TileMap *tm, SDL_Renderer *renderTarget_) : MapObject(tm, renderTarget_)
{
	flinching = false;
	bombaStarting = false;
    bombaHolding = false;
    bombaThrowing = false;
	kicking = false;
	dead = false;
	onGround = false;
	jumpAction = false;
	hold = false;
	dying = false;
	falling = true;	

	visible = true;	
	start = true;

	flinchTimer = 0;

	width = 25;
    height = 27;
    cwidth = 13;
    cheight = 28;//27;

    maxSpeed = 1.6;
	moveSpeed = maxSpeed;
    stopSpeed = maxSpeed;

    fallSpeed = 0.15;
    maxFallSpeed = 4.0;
    jumpStart = -2.5;
    stopJumpSpeed = 0.3;

    facingRight = true;

	maxHealth = 1;
    health = maxHealth;

	kickDamage = 1;
	kickRange = 13;


	activeBomba = NULL;
	maxBombaCount = 1;
	bombaCount = maxBombaCount;
	bombaDamage = 10;//?


	//load sprites
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
	
	animationTexture = LoadTexture("./Resources/Sprites/Player/GoS_PlayerSprites.bmp", renderTarget);
    if (animationTexture)
        printf("DEBUG: Loaded player textures\n");
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

	currentAction = DANCING;
    animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
    animation.setDelay(200);
    width = sprite_rects[currentAction][animation.getFrame()].w;
	danceTimer = 3001;

	//Audio loading
    player_sounds = NULL;
    player_sounds = new AudioPlayer*[SFX_NUM];
    for (int i=0; i<SFX_NUM; i++)
    {
        player_sounds[i] = NULL;
    }
	player_sounds[DANCE_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_DanceAnim.wav", false);
	player_sounds[WALK_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_Footstep.wav", false);
    player_sounds[JUMP_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_Jump.wav", false);
	player_sounds[KICK_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_Kick.wav", false);
	player_sounds[HIT_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_Hit.wav", false);
    player_sounds[DIE_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_Die.wav", false);
	player_sounds[BOMBA_START_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_TakeOutBomba.wav", false);
	player_sounds[BOMBA_FUSE_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_BombaFuse.wav", false);
	player_sounds[BOMBA_THROW_SFX] = new AudioPlayer("./Resources/SFX/scratch.wav", false);
	player_sounds[BOMBA_HIT_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_BombaExplodes.wav", false);

	
}
GoS_Player::~GoS_Player()
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


	if (activeBomba)
		delete activeBomba;
	activeBomba = NULL;

	std::list<Bomba*>::iterator bombas_iter;
    for (bombas_iter=bombas.begin(); bombas_iter != bombas.end(); )
    {
        Bomba *bomba = *bombas_iter;
       	delete bomba;
       	*bombas_iter = NULL;
       	bombas_iter = bombas.erase(bombas_iter);
    }
	/*
    std::list<FireBall*>::iterator iter;
    for (iter=fireBalls.begin(); iter != fireBalls.end(); ++iter)
    {
        FireBall *fb = (*iter);
        delete fb;
        (*iter) = NULL;
    }
	*/
    for (int i=0; i<SFX_NUM; i++)
    {
        delete player_sounds[i];
        player_sounds[i] = NULL;
    }
    delete player_sounds;
    player_sounds = NULL;
}
void GoS_Player::bombaAction()
{
	if (!isBusy() && bombaCount > 0)
	{
		bombaStarting = true;
		if (activeBomba)
			delete activeBomba;
		activeBomba = new Bomba(tileMap, renderTarget, facingRight, 10);
		activeBomba->setVector(0, 0);
		activeBomba->setPosition(x, y);
		bombaCount--;
		if (bombaCount < 0)
			bombaCount = 0;
	}
	if (currentAction == BOMBA_START && animation.hasPlayedOnce())
	{
		bombaStarting = false;
		bombaThrowing = true;
	}
}
void GoS_Player::setHold(bool b)
{
	
	if (onGround && !isBusy())
	{
		hold = b;	
	}
	else
		hold = false;
}
bool GoS_Player::isBusy()
{
	return (jumpAction || currentAction == LANDING || kicking || bombaStarting || bombaThrowing || hold || dying || start);
}
void GoS_Player::getNextPosition()
{
	//movement
	//bool moveCondition = !jumpAction && currentAction != LANDING && !kicking;
	if (/*!jumpAction && currentAction != LANDING*/ /*moveCondition*/ !isBusy())
	{
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
	}
/*
	//Jumping
    if (jumping && !falling)
    {
        //player_sounds[JUMP_SFX]->play(0);
        dy = jumpStart;
        falling = true;
    }
*/
	
	if (currentAction == KICKING)
	{
		if (kicking)
			dx = 0;
		if (animation.getFrame() == 5)
		{
			if (facingRight)
				x = kickStartX + 1;
			else
				x = kickStartX - 1;
		}
		if (animation.getFrame() == 6)
        {
            if (facingRight)
                x = kickStartX + 2;
            else
                x = kickStartX - 2;
        }
		if (animation.getFrame() == 7)
        {
            if (facingRight)
                x = kickStartX + 3;
            else
                x = kickStartX - 3;
        }
	}

	jumpInit = false;	
	if (!isBusy())
	{
	if (onGround && jumping && currentAction != LANDING)
	{
		onGround = false;
		jumpInit = true;
		jumpAction = true;
		dy = jumpStart;
	}
	}
	//falling
    if (true)
    {
        dy += fallSpeed;

 //       if (dy > 0)
 //           jumping = false;
 //       if (dy < 0 && !jumping)
 //           dy += stopJumpSpeed;
        if (dy > maxFallSpeed)
            dy = maxFallSpeed;
    }

}
void GoS_Player::reset(double x, double y)
{
	flinching = false;
    bombaStarting = false;
    bombaHolding = false;
    bombaThrowing = false;
    kicking = false;
    dead = false;
    onGround = false;
    jumpAction = false;
	hold = false;
	dying = false;
	visible = true;
	start = true;

    flinchTimer = 0;
	health = maxHealth;

	left = false;
    right = false;
    up = false;
    down = false;
    jumping = false;
    falling = false;
	
	setVector(0, 0);
	setPosition(x, y);

	currentAction = DANCING;
    animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
    animation.setDelay(200);
    width = sprite_rects[currentAction][animation.getFrame()].w;
	danceTimer = 3001;

	if (activeBomba)
		delete activeBomba;
	activeBomba = NULL;
	std::list<Bomba*>::iterator bombas_iter;
    for (bombas_iter=bombas.begin(); bombas_iter != bombas.end(); )
    {
        Bomba *bomba = *bombas_iter;
        delete bomba;
        *bombas_iter = NULL;
        bombas_iter = bombas.erase(bombas_iter);
    }
}
void GoS_Player::hit(int damage)
{
    if (flinching || dying || dead)
        return;
    health -= damage;
    if (health <= 0)
    {
        health = 0;
        dying = true;
    }
    flinching = true;
    flinchTimer = getMs();

   	dy = (jumpStart/2);
	if (facingRight)
	{
		dx = -maxSpeed/2;
	}
	else
	{
		dx = maxSpeed/2;
	}

    bombaStarting = false;
    bombaHolding = false;
    bombaThrowing = false;
    kicking = false;
    jumpAction = false;
    hold = false;
	if (activeBomba)
	{
		delete activeBomba;
		activeBomba = NULL;
	}
}
void GoS_Player::update()
{
	//update position
    getNextPosition();
    //checkTileMapCollision();
    //setPosition(xtemp, ytemp);
	xdest = x + dx;
	ydest = y + dy;


	while (1)
	{
		if (tileMap->checkTileCollision(int(xdest+cwidth/2), (int)y))
		{
			Tile *tile = tileMap->getTileFromPos(int(xdest+cwidth/2), (int)y);
			if (tile->getType() == Tile::BLOCKED)
			{
				xdest--;	
			}
		}
		else
			break;
	}
	while (1)
    {
        if (tileMap->checkTileCollision(int(xdest-cwidth/2), (int)y))
        {
            Tile *tile = tileMap->getTileFromPos(int(xdest-cwidth/2), (int)y);
            if (tile->getType() == Tile::BLOCKED)
            {
                xdest++;
            }
        }
        else
            break;
    }
	if (xdest - cwidth/2 < 0)
		xdest = cwidth/2;
	x = xdest;

	//x += dx;
	//y += dy;
	
	//int x_dest = (int)x;
	//int y_dest = (int)(y/*dest*/+cheight/2 - 1);
	bool ground_collision = false;
	onGround = false;
	while (tileMap->checkTileCollision(xdest, ydest+cheight/2 - 1))
	{
		//printf("Collision!!\n");
		ydest -= 1;
		ground_collision = true;
		falling = false;
		onGround = true;
		jumpAction = false;
		dx = 0;
		//y--;
	}
	bool upper_collision = false;
	int collision_offset = 6;
	while (tileMap->checkTileCollision(xdest - cwidth/2, ydest-cheight/2 + collision_offset) || tileMap->checkTileCollision(xdest + cwidth/2, ydest-cheight/2 + collision_offset))
	{
		ydest += 1;
		dy = 0;
		upper_collision = true;
	}
	if (upper_collision)
	{
		if (tileMap->checkTileCollision(xdest + dx - cwidth/2, ydest-cheight/2 + collision_offset) || tileMap->checkTileCollision(xdest + dx + cwidth/2, ydest-cheight/2 + collision_offset))
			dx = 0;
	}
	//	ydest += 1;
	y = ydest;
	//if (tileMap->checkTileCollision((int)x, (int)(y+cheight/2)))
	//	printf("Collision!\n");


	if (activeBomba)
	{
		activeBomba->update();
		if (activeBomba->shouldRemove())
		{
			delete activeBomba;
			activeBomba = NULL;
		}
	}
	std::list<Bomba*>::iterator bombas_iter;
    for (bombas_iter=bombas.begin(); bombas_iter != bombas.end(); )
    {
        Bomba *bomba = *bombas_iter;
        bomba->update();
        if (bomba->shouldRemove())
        {
			printf("Number of channels: %d\n", Mix_AllocateChannels(-1));
			player_sounds[BOMBA_FUSE_SFX]->stop(7);
			player_sounds[BOMBA_HIT_SFX]->play(0);
            delete bomba;
            *bombas_iter = NULL;
            bombas_iter = bombas.erase(bombas_iter);
        }
        else
        {
            ++bombas_iter;
        }
    }

	//Set Animations and junk
	if (currentAction == KICKING)
	{
		if (animation.hasPlayedOnce())
		{
			kicking = false;
		}
	}
	if (currentAction == BOMBA_THROW)
	{
		if (animation.hasPlayedOnce())
		{
			bombaThrowing = false;
		}
	}
	if (currentAction == DYING)
	{
		if (animation.hasPlayedOnce())
		{
			dead = true;
		}
	}

	if (jumpInit)
	{
		currentAction = JUMPING_START;
        animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
        animation.setDelay(75);
        width = sprite_rects[currentAction][animation.getFrame()].w;

		player_sounds[JUMP_SFX]->play(0);
	}
	if (currentAction == JUMPING_START)
	{
		if (ground_collision)
		{
			currentAction = LANDING;
            animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
            animation.setDelay(80);
            width = sprite_rects[currentAction][animation.getFrame()].w;

		}
		else if (animation.hasPlayedOnce())
		{
			currentAction = JUMPING_HOLD;
			animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
			animation.setDelay(-1);
			width = sprite_rects[currentAction][animation.getFrame()].w;
		}
	}
	if (currentAction == JUMPING_HOLD)
    {
		if (ground_collision)
		{
			currentAction = LANDING;
            animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
            animation.setDelay(80);
            width = sprite_rects[currentAction][animation.getFrame()].w;

		}
        else if (tileMap->checkTileCollision(xdest, ydest + cheight/2 + 8))
        {
            currentAction = JUMPING_LAND;
            animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
            animation.setDelay(-1);
            width = sprite_rects[currentAction][animation.getFrame()].w;
        }
    }
	if (currentAction == JUMPING_LAND)
    {
        if (ground_collision)
        {
            currentAction = LANDING;
            animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
            animation.setDelay(75);
            width = sprite_rects[currentAction][animation.getFrame()].w;
        }
    }
	if (currentAction == LANDING)
    {
        if (animation.hasPlayedOnce())
        {
            currentAction = IDLE;
            animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
            animation.setDelay(-1);
            width = sprite_rects[currentAction][animation.getFrame()].w;
        }
    }

	
	//check done flinching
    if (flinching)
    {
        double elapsed = getMs() - flinchTimer;
        if (elapsed > 1000)
            flinching = false;
    }


	//Set Animation
	if (start)
	{
		if (left || right)
		{
			if (currentAction != FRONT_FACE)
			{
				currentAction = FRONT_FACE;
                animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
                animation.setDelay(500);
                width = sprite_rects[currentAction][animation.getFrame()].w;

				if (right)
					facingRight = true;
				else if (left)
					facingRight = false;
			}
		}
		if (currentAction == FRONT_FACE)
		{
			if (animation.hasPlayedOnce())
			{
				start = false;
			}
			danceTimer = getMs();
		}
		else if (currentAction == DANCING)
		{
            if (animation.frameChanged() && animation.getFrame() == 1)
            {
                player_sounds[DANCE_SFX]->play(0);
            }

		}
	}
	else if (dying)
    {
        if (!onGround)
        {
			if (currentAction != HIT)
			{
            	currentAction = HIT;
            	animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
            	animation.setDelay(-1);
            	width = sprite_rects[currentAction][animation.getFrame()].w;

				player_sounds[HIT_SFX]->play(0);
			}
        }
        else
        {
            if (currentAction != DYING)
            {
                currentAction = DYING;
                animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
                animation.setDelay(850);
                width = sprite_rects[currentAction][animation.getFrame()].w;
            }
			else
			{
				if (animation.frameChanged() && animation.getFrame() == 1)
				{
					player_sounds[DIE_SFX]->play(0);
				}
			}
        }
    }
	else if (kicking)
	{
		if (currentAction != KICKING)
		{
			currentAction = KICKING;
            animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
            animation.setDelay(65);
            width = sprite_rects[currentAction][animation.getFrame()].w;
			kickStartX = x;
		}
		else
		{
			if (animation.frameChanged() && animation.getFrame() == 3)
			{
				player_sounds[KICK_SFX]->play(0);
			}
		}
		danceTimer = getMs();
	}
	else if (bombaStarting)
	{
		if (currentAction != BOMBA_START)
		{
			currentAction = BOMBA_START;
            animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
            animation.setDelay(50);
            width = sprite_rects[currentAction][animation.getFrame()].w;
			player_sounds[BOMBA_START_SFX]->play(0);
			player_sounds[BOMBA_FUSE_SFX]->play(AudioPlayer::LOOP, 7);
		}
		if (currentAction == BOMBA_START && activeBomba)
		{
			switch (animation.getFrame())
			{
				case 0:
					if (facingRight)
						activeBomba->setPosition(x+11, y-9);
					else
						activeBomba->setPosition(x-11, y-9);
					break;
				case 1:
					if (facingRight)
						activeBomba->setPosition(x+4, y-21);
					else
						activeBomba->setPosition(x-4, y-21);
					break;
				case 2:
					if (facingRight)
						activeBomba->setPosition(x+1, y-21);
					else
						activeBomba->setPosition(x-1, y-21);
					break;
			}
			//if (animation.getFrame() == 0)
			//{
			//	activeBomba->setPosition(x+12, y-8);
			//}
			
		}
		danceTimer = getMs();
	}
	else if (bombaThrowing)
	{
		if (currentAction != BOMBA_THROW)
        {
            currentAction = BOMBA_THROW;
            animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
            animation.setDelay(50);
            width = sprite_rects[currentAction][animation.getFrame()].w;
			player_sounds[BOMBA_THROW_SFX]->play(0);
        }
		if (currentAction == BOMBA_THROW && activeBomba)
        {
            switch (animation.getFrame())
            {
                case 0:
					if (facingRight)
                    	activeBomba->setVector(2.5, 0);
					else
						activeBomba->setVector(-2.5, 0);
					activeBomba->setActive(true);
					bombas.push_back(activeBomba);
					activeBomba = NULL;
                    break;
                case 1:
                    //activeBomba->setPosition(x+4, y-21);
                    break;
                case 2:
                    //activeBomba->setPosition(x+1, y-21);
                    break;
            }

        }
		danceTimer = getMs();
	}
	else if (jumpAction || currentAction == LANDING)
	{
		/*
		if (currentAction != JUMPING_START && currentAction != JUMPING_HOLD)
		{
			currentAction = JUMPING_START;
            animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
            animation.setDelay(80);
            width = sprite_rects[currentAction][animation.getFrame()].w;
		}
		*/
		danceTimer = getMs();
	}
	else if (left || right)
	{
		if (currentAction != WALKING)
        {
            currentAction = WALKING;
            animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
            animation.setDelay(300);
            width = sprite_rects[currentAction][animation.getFrame()].w;
        }
		else
		{
			if (animation.frameChanged())
			{
				if ((animation.getFrame() == 0 && animation.hasPlayedOnce()) || (animation.getFrame() == 1))
				{
					player_sounds[WALK_SFX]->play(0);	
				}
			}
		}
		danceTimer = getMs();
	}
	else
	{
		double elapsed = getMs() - danceTimer;
		if (elapsed < 3000)
		{
			if (currentAction != IDLE)
        	{
				//if (currentAction != LANDING)
				//{
            		currentAction = IDLE;
            		animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
            		animation.setDelay(400);
            		width = sprite_rects[currentAction][animation.getFrame()].w;
				//}
        	}
		}
		else
		{
			if (currentAction != DANCING)
			{
				currentAction = DANCING;
            	animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
            	animation.setDelay(200);
            	width = sprite_rects[currentAction][animation.getFrame()].w;
			}
			else
			{
				if (animation.frameChanged() && animation.getFrame() == 1)
				{
					player_sounds[DANCE_SFX]->play(0);
				}
			}
		}
	}

	//Hold overwrites everything (need to make sure to only set when already on ground and not busy
	if (hold)
	{
		currentAction = IDLE;
       	animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
        animation.setDelay(400);
        width = sprite_rects[currentAction][animation.getFrame()].w;
		danceTimer = getMs();
	}


	animation.update();
	//if (currentAction != BOMBA_THROW)	
	//{
		//set direction
	if (!isBusy())
	{
        if (right)
            facingRight = true;
        else if (left)
            facingRight = false;
	}
	//}
}
void GoS_Player::draw()
{
	if (!visible)
		return;

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
	if (activeBomba)
		activeBomba->draw();
	std::list<Bomba*>::iterator bombas_iter;
    for (bombas_iter=bombas.begin(); bombas_iter != bombas.end();  ++bombas_iter)
    {
        Bomba *bomba = *bombas_iter;
        bomba->draw();
    }
}

void GoS_Player::checkAttack(std::list<Enemy*> enemies)
{
	std::list<Enemy*>::iterator enemy_iter;
	std::list<Bomba*>::iterator bombas_iter;
	for (enemy_iter=enemies.begin(); enemy_iter != enemies.end(); ++enemy_iter)
	{
		Enemy *enemy = *enemy_iter;
		if (kicking && (animation.getFrame() >= 5 && animation.getFrame() <= 7))		
		{
			SDL_Rect eRect = enemy->getCollisionRect();
            SDL_Rect aRect;

			if (facingRight)
            {
                aRect.x = (int)x;
                aRect.y = (int)(y - height/2);
                aRect.w = kickRange;
                aRect.h = height;
            }
            else
            {
                aRect.x = (int)(x - kickRange);
                aRect.y = (int)(y - height/2);
                aRect.w = kickRange;
                aRect.h = height;
            }

			if (rectOverlap(aRect, eRect))
            {
                enemy->hit(kickDamage);
            }
		}

		for (bombas_iter=bombas.begin(); bombas_iter != bombas.end();  ++bombas_iter)
		{
			Bomba *bomba = *bombas_iter;
			enemy->checkBombaCollision(bomba);
		}

		if (intersects(enemy))
		{
			hit(1);
		}	
	}
}

