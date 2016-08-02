#pragma once

#include <SDL2/SDL.h>
#include "../headers/MapObject.h"

class BgObject : public MapObject
{
	private:
		SDL_Texture *animationTexture;
        SDL_Rect *sprite_rects;
        int numSprites;

		static const char* BG_OBJECT_FILE;// = "./Resources/Files/BgObject_List.txt";

	public:
		BgObject(TileMap *tm, SDL_Renderer *renderTarget_, int id);
        ~BgObject();

		void update();
        void draw();
		
};
