#include <stdlib.h>
#include <stdio.h>
#include "../headers/BgObject.h"
#include "../headers/SDL_Helpers.h"

const char* BgObject::BG_OBJECT_FILE = "./Resources/Files/BgObject_List.txt";
BgObject::BgObject(TileMap *tm, SDL_Renderer *renderTarget_, int id) : MapObject(tm, renderTarget_)
{
	FILE* fp = NULL;
	fp = fopen(BG_OBJECT_FILE, "r");
	int num_objects;
	fscanf(fp, "%d ", &num_objects);
	if (id >= num_objects)
		printf("Error: BG Object Id outside of bounds\n");
	int id_, frameNum_, width_, height_, delay;
	char filename[128];

	bool match = false;
	for (int  i=0; i<num_objects; i++)
	{
		fscanf(fp, "%d %d %d %d %d %s", &id_, &frameNum_, &width_, &height_, &delay, filename);
		if (id == id_)
		{
			match = true;
			break;
		}
	}
	if (!match)
		printf("Error: Failed to find BG Object Id(%d) specfied\n", id);
	fclose(fp);

	dx = 0;
    dy = 0;

    moveSpeed = 0;
    maxSpeed = 0;
    fallSpeed = 0;
    maxFallSpeed = 0;

    width = width_;
    height = height_;
    cwidth = -1;
    cheight = -1;


	animationTexture = NULL;
    animationTexture = LoadTexture(filename, renderTarget);
	numSprites = frameNum_;
    sprite_rects = NULL;
    sprite_rects = new SDL_Rect[numSprites];
    for (int i=0; i<numSprites; i++)
    {
        //sprite_rects[i] = {i*width, 0, width, height};
        sprite_rects[i].x = i*width;
        sprite_rects[i].y = 0;
        sprite_rects[i].w = width;
        sprite_rects[i].h = height;
    }
    animation.setFrames(animationTexture, sprite_rects, numSprites);
    animation.setDelay(delay);

    right = true;
    facingRight = right;

}
BgObject::~BgObject()
{
    delete [] sprite_rects;
    sprite_rects = NULL;

    SDL_DestroyTexture(animationTexture);
    animationTexture = NULL;
}
void BgObject::update()
{
	animation.update();
}
void BgObject::draw()
{
	setMapPosition();
    mapObjectDraw();
}
