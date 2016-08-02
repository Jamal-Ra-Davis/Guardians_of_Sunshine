#include <time.h>
#include <math.h>
#include "../headers/Animation.h"
#include "../headers/SDL_Helpers.h"

Animation::Animation()
{
	playedOnce = false;
	playsOnce = false;
	loop_counter = 0;
}
void Animation::setFrames(SDL_Texture *texture_, SDL_Rect *cropFrames_, int numFrames_, bool playsOnce_, int loopNum_)
{
	texture = texture_;
	cropFrames = cropFrames_;
	currentFrame = 0;
	prevFrame = 0;
	numFrames = numFrames_;

	startTime = getMs();
	playedOnce = false;
	playsOnce = playsOnce_;

	loopNum = loopNum_;
	if (playsOnce)
		loopNum = 1;
}
/*
void Animation::update()
{
	if (delay == -1)
		return;
	if (playsOnce && playedOnce)
		return;		

	int tempFrame = currentFrame;
	double elapsed = (getMs() - startTime);
	if (elapsed > delay)
	{
		currentFrame++;
		startTime = getMs();
	}
	if (currentFrame == numFrames)
	{
		if (!playsOnce)
			currentFrame = 0;
		else
			currentFrame = tempFrame;
		playedOnce = true;
	}
}
*/

void Animation::update()
{
    if (delay == -1)
        return;
    if ((loopNum > 0) && playedOnce)
        return;

    int tempFrame = currentFrame;
	prevFrame = currentFrame;
    double elapsed = (getMs() - startTime);
    if (elapsed > delay)
    {
        currentFrame++;
        startTime = getMs();
    }
    if (currentFrame == numFrames)
    {
        if (!(loopNum > 0))
		{
            currentFrame = 0;
			playedOnce = true;
		}
        else
		{
			loop_counter++;
			if (loop_counter >= loopNum)
			{
				currentFrame = tempFrame;
				playedOnce = true;
			}
			else
            	currentFrame = 0;
		}
        //playedOnce = true;
    }
}


