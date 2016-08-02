#pragma once
#include <SDL2/SDL.h>
#include <string>


class Background
{
	private:
		SDL_Texture *bgImage;
		double x, y;
		double dx, dy;
		double moveScale;
		int textureWidth, textureHeight;
		SDL_Renderer *renderTarget;
	public:
		Background(std::string s, double ms, SDL_Renderer *renderTarget_);
		~Background();
		void setPosition(double x_, double y_);
		void setVector(double dx_, double dy_);
		void update();
		void draw();
};


