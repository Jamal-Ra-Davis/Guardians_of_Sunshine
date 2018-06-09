#include "../headers/ComboMove.h"
#include "../headers/SDL_Helpers.h"

ComboMove::ComboMove(TileMap *tm, SDL_Renderer *renderTarget_) : MapObject(tm, renderTarget_)
{
	visible = false;
    active = false;
	active_trigger = false;
    spin = false;
    combo_time = 15000;//~8 seconds 
	combo_pass = false;
	combo_fail = false;

	width = 25;
    height = 27;
    cwidth = 25;
    cheight = 27;

	maxSpeed = 0;
    moveSpeed = maxSpeed;
    stopSpeed = maxSpeed;

	fallSpeed = 0;
    maxFallSpeed = 0;
    jumpStart = 0;
    stopJumpSpeed = 0;

    facingRight = true;

	//load sprites
    numAnimations = 13;
    frameNumbers = NULL;
    frameNumbers = new int[numAnimations];
    int tempNums[13] = {1, 3, 3, 3, 3, 5, 3, 1, 3, 3, 3, 3, 5};
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


	animationTexture = LoadTexture("./Resources/Sprites/Player/GoS_ComboMoves.bmp", renderTarget);
    if (animationTexture)
        printf("DEBUG: Loaded combo move textures\n");
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
    animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
    animation.setDelay(500);
    width = sprite_rects[currentAction][animation.getFrame()].w;


	
	action_idx = 1;
}
ComboMove::~ComboMove()
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
}
void ComboMove::setLeft(bool b)
{
	if (isBusy())
		return;
	left = b;
	inputBuffer.add(InputBuffer::LEFT);
}
void ComboMove::setRight(bool b)
{
	if (isBusy())
        return;
    right = b;
	inputBuffer.add(InputBuffer::RIGHT);
}
void ComboMove::setUp(bool b)
{
	if (isBusy())
        return;
    up = b;
	inputBuffer.add(InputBuffer::UP);
}
void ComboMove::setDown(bool b)
{
	if (isBusy())
        return;
    down = b;
	inputBuffer.add(InputBuffer::DOWN);
}
void ComboMove::setJumping(bool b)
{
	if (isBusy())
        return;
    jumping = b;
	inputBuffer.add(InputBuffer::JUMP);
}
void ComboMove::setSpin(bool b)
{
    if (isBusy())
        return;
    spin = b;
	inputBuffer.add(InputBuffer::SPIN);
}
bool ComboMove::isBusy()
{
	return (up || down || left || right || jumping || spin || combo_pass || combo_fail);
}
void ComboMove::reset()
{
	visible = false;
    active = false;
    active_trigger = false;
    spin = false;
	combo_pass = false;
    combo_fail = false;
	inputBuffer.reset();
}
void ComboMove::onActivate()
{
	start_time = getMs();
	inputBuffer.reset();
	up = false;
   	down = false;
    left = false;
    right = false;
    jumping = false;
    spin = false;
}
void ComboMove::update()
{
	//active = true;
	if (!active)
		return;
	if (combo_fail)
		return;
	if (!active_trigger)
	{
		onActivate();
	}

	inputBuffer.checkLists();
    if (inputBuffer.foundFullMatch())
    {
        combo_pass = true;
    }

	if (getMs() - start_time >= combo_time && !isBusy() && !combo_pass)
	{
		//printf("diff = %lf, combo_time = %lf\n", getMs() - start_time, combo_time);
		combo_fail = true;
		inputBuffer.printOutput();
	}
	bool danger_zone = (getMs() - start_time) >= (combo_time * 0.65);

	/*
	if (currentAction == IDLE)
	{
		if (animation.hasPlayedOnce())
		{
			currentAction = action_idx;
            animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
			if (currentAction == JUMP)
            	animation.setDelay(60);
			else
				animation.setDelay(100);
            width = sprite_rects[currentAction][animation.getFrame()].w;
			action_idx++;
			if (action_idx >= NUM_COMBO_MOVES)
				action_idx = 1;
		}
	}
	else
	{
		if (animation.hasPlayedOnce())
		{
			currentAction = IDLE;
    		animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
    		animation.setDelay(250);
    		width = sprite_rects[currentAction][animation.getFrame()].w;
		}
	}
	*/

	if (currentAction != IDLE)
	{
		if (animation.hasPlayedOnce())
        {
			up = false;
			down = false;
			left = false;
			right = false;
			jumping = false;
			spin = false;
        }
	}	


	if (up)
	{
		if (!danger_zone)
		{
			if (currentAction != UP)
			{
				currentAction = UP;
            	animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
            	animation.setDelay(100);
            	width = sprite_rects[currentAction][animation.getFrame()].w;
			}
		}
		else
		{
			if (currentAction != SAD_UP)
            {
                currentAction = SAD_UP;
                animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
                animation.setDelay(100);
                width = sprite_rects[currentAction][animation.getFrame()].w;
            }
		}
	}
	else if (down)
	{
		if (!danger_zone)
		{
			if (currentAction != DOWN)
        	{
            	currentAction = DOWN;
            	animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
            	animation.setDelay(100);
            	width = sprite_rects[currentAction][animation.getFrame()].w;
        	}
		}
		else
		{
			if (currentAction != SAD_DOWN)
            {
                currentAction = SAD_DOWN;
                animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
                animation.setDelay(100);
                width = sprite_rects[currentAction][animation.getFrame()].w;
            }
		}
	}
	else if (right)
	{
		if (!danger_zone)
		{
			if (currentAction != RIGHT)
        	{
            	currentAction = RIGHT;
            	animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
            	animation.setDelay(100);
            	width = sprite_rects[currentAction][animation.getFrame()].w;
        	}
		}
		else
		{
			if (currentAction != SAD_RIGHT)
            {
                currentAction = SAD_RIGHT;
                animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
                animation.setDelay(100);
                width = sprite_rects[currentAction][animation.getFrame()].w;
            }
		}
	}
	else if (left)
	{
		if (!danger_zone)
		{
			if (currentAction != LEFT)
        	{
            	currentAction = LEFT;
            	animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
            	animation.setDelay(100);
            	width = sprite_rects[currentAction][animation.getFrame()].w;
        	}
		}
		else
		{
			if (currentAction != SAD_LEFT)
            {
                currentAction = SAD_LEFT;
                animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
                animation.setDelay(100);
                width = sprite_rects[currentAction][animation.getFrame()].w;
            }
		}
	}
	else if (jumping)
	{
		if (!danger_zone)
		{
			if (currentAction != JUMP)
        	{
            	currentAction = JUMP;
            	animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
            	animation.setDelay(60);
            	width = sprite_rects[currentAction][animation.getFrame()].w;
        	}
		}
		else
		{
			if (currentAction != SAD_JUMP)
            {
                currentAction = SAD_JUMP;
                animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
                animation.setDelay(60);
                width = sprite_rects[currentAction][animation.getFrame()].w;
            }
		}
	}
	else if (spin)
	{
		if (currentAction != SPIN)
        {
            currentAction = SPIN;
            animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
            animation.setDelay(100);
            width = sprite_rects[currentAction][animation.getFrame()].w;
        }
	}
	else
	{
		if (!danger_zone || combo_pass)
		{
			if (currentAction != IDLE)
        	{
            	currentAction = IDLE;
            	animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
            	animation.setDelay(-1);
            	width = sprite_rects[currentAction][animation.getFrame()].w;
        	}
		}
		else
		{
			if (currentAction != SAD_IDLE)
            {
                currentAction = SAD_IDLE;
                animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
                animation.setDelay(-1);
                width = sprite_rects[currentAction][animation.getFrame()].w;
            }
		}
	}

	animation.update();
	active_trigger = true;
}
void ComboMove::draw()
{
	if (!visible)
        return;

    setMapPosition();
    mapObjectDraw();
}



