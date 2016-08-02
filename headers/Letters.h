#pragma once
#include <SDL2/SDL.h>
#include <string>

class Letters
{
	private:
		std::string word;
		int spacing;
		int x, y;
		bool visible;
		int xOffset, yOffset;
		static const int LETTER_WIDTH=6;
		static const int LETTER_HEIGHT = 13;		

		SDL_Texture *letterTexture;
		SDL_Renderer *renderTarget;

	public:
		Letters(SDL_Renderer *renderTarget_, std::string word_, int spacing_, int x_, int y_, bool visible_=true);
		~Letters();
		int getWordLength() {return word.length();}
		void setWord(std::string word_) {word = word_;}
		int getSpacing() {return spacing;}
		void setSpacing(int s) {spacing = s;}
		bool getVisible() {return visible;}
        void setVisible(bool b) {visible = b;}
		int getX() {return x;}
		int setX(int x_) {x = x_;}
		int getY() {return y;}
		int setY(int y_) {y = y_;}
		int getXOffset() {return xOffset;}
		void setXOffset(int offset) {xOffset = offset;}
		int getYOffset() {return yOffset;}
        void setYOffset(int offset) {yOffset = offset;}
		int getWidth() {return getWordLength()*LETTER_WIDTH + (getWordLength()-1)*spacing;}
		int getHeight() {return LETTER_HEIGHT;}
		void draw();
};
