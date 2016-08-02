#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>

#include "../headers/SDL_Helpers.h"
#include "../headers/GamePanel.h"

#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 240


int main(int nargs, char *args[])
{
	GamePanel gPanel;
	gPanel.setWindowName("Guardians of Sunshine");
	gPanel.run();


	return 0;
}
