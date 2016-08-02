#include "../headers/Poof.h"
#include "../headers/SDL_Helpers.h"
#include "../headers/Numbers.h"


Poof::Poof(SDL_Renderer *renderTarget_, int x_, int y_, int score_)
{
	x = x_;
    y = y_;

	score = score_;
	loopNum = 3;	

    renderTarget = renderTarget_;

    remove = false;

    width = 46;
    height = 48;


	//load sprites
    numAnimations = 2;
    frameNumbers = NULL;
    frameNumbers = new int[numAnimations];
    int tempNums[2] = {3, 3};
    for (int i=0; i<numAnimations; i++)
        frameNumbers[i] = tempNums[i];

    sprite_rects = NULL;
    sprite_rects = new SDL_Rect*[numAnimations];
    for (int i=0; i<numAnimations; i++)
    {
        sprite_rects[i] = NULL;
        sprite_rects[i] = new SDL_Rect[frameNumbers[i]];
    }

	animationTexture = NULL;
    animationTexture = LoadTexture("./Resources/Sprites/Enemies/GoS_EnemyPoof.bmp", renderTarget);
    if (animationTexture)
        printf("DEBUG: Loaded poof animation textures\n");
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

	currentAction = START;
    animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], false, 3);
    animation.setDelay(70);
    width = sprite_rects[currentAction][animation.getFrame()].w;

}
Poof::~Poof()
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
void Poof::setMapPosition(int x_, int y_)
{
	xmap = x_;
    ymap = y_;
}
void Poof::update()
{
	animation.update();
	if (currentAction == START && animation.hasPlayedOnce())
	{
		currentAction = END;
        animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
        animation.setDelay(40);
        width = sprite_rects[currentAction][animation.getFrame()].w;
	}
	if (currentAction == END && animation.hasPlayedOnce())
	{
		remove = true;
	}
	
}
void Poof::draw()
{
	SDL_Rect posRect = {(int)(x + xmap - width/2), (int)(y + ymap - height/2), width, height};
    SDL_Rect cropRect = animation.getImageRect();
    SDL_RenderCopy(renderTarget, animation.getFrameTexture(), &cropRect, &posRect);

	Numbers numbers(renderTarget, 3, 2, x+xmap, y+ymap);
	numbers.setX(x+xmap - numbers.getWidth()/2);
    numbers.setY(y+ymap - numbers.getHeight()/2);
	numbers.draw(score);
}
