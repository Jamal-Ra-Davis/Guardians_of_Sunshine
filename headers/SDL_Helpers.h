#pragma once
#include <SDL2/SDL.h>
#include <string>


SDL_Texture* LoadTexture(std::string filePath, SDL_Renderer *renderTarget);
void PrintEvent(const SDL_Event * event);
bool valInRange(int val, int min, int max);
bool rectOverlap(SDL_Rect a, SDL_Rect b);
double getMs();
