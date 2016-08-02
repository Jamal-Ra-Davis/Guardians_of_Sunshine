#pragma once
#include <SDL2/SDL.h>

class Animation
{
	private:
		SDL_Texture *texture;
		int currentFrame;
		int prevFrame;
		SDL_Rect *cropFrames;
		int numFrames;

		double startTime;
		long delay;

		bool playedOnce;
		bool playsOnce;

		int loopNum;
		int loop_counter;

	public:
		Animation();
	
		int getFrame() {return currentFrame;}	
		SDL_Texture *getFrameTexture() {return texture;}
		SDL_Rect getImageRect() {return cropFrames[currentFrame];}
		bool hasPlayedOnce() {return playedOnce;}
		bool frameChanged() {return (currentFrame != prevFrame);}

		void setFrames(SDL_Texture *texture, SDL_Rect *cropFrames, int numFrames_, bool playsOnce=false, int loopNum_ = -1);
		void setDelay(long d) {delay = d;}
		void setFrame(int i) {currentFrame = i;}
		void update();
};

