//#include <SDL2/SDL.h>
#include <stdio.h>
#include <sys/time.h>
//#include <string>
#include "../headers/SDL_Helpers.h"

SDL_Texture* LoadTexture(std::string filePath, SDL_Renderer *renderTarget)
{
	SDL_Texture *texture = NULL;
    SDL_Surface *surface = SDL_LoadBMP(filePath.c_str());

    if (surface == NULL)
    {
        printf("Failed to load image error: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return NULL;
    }


    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));
    texture = SDL_CreateTextureFromSurface(renderTarget, surface);
    if (texture == NULL)
    {
        printf("Failed to convert surface error: %s\n", SDL_GetError());
		printf("%s failed to load\n", filePath.c_str());
        SDL_FreeSurface(surface);
        return NULL;
    }
    SDL_FreeSurface(surface);
    return texture;
}
void PrintEvent(const SDL_Event * event)
{
    if (event->type == SDL_WINDOWEVENT) {
        switch (event->window.event) {
        case SDL_WINDOWEVENT_SHOWN:
            SDL_Log("Window %d shown", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            SDL_Log("Window %d hidden", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            SDL_Log("Window %d exposed", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_MOVED:
            SDL_Log("Window %d moved to %d,%d",
                    event->window.windowID, event->window.data1,
                    event->window.data2);
            break;
        case SDL_WINDOWEVENT_RESIZED:
            SDL_Log("Window %d resized to %dx%d",
                    event->window.windowID, event->window.data1,
                    event->window.data2);
            break;
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            SDL_Log("Window %d size changed to %dx%d",
                    event->window.windowID, event->window.data1,
                    event->window.data2);
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            SDL_Log("Window %d minimized", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            SDL_Log("Window %d maximized", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_RESTORED:
            SDL_Log("Window %d restored", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_ENTER:
            SDL_Log("Mouse entered window %d",
                    event->window.windowID);
            break;
        case SDL_WINDOWEVENT_LEAVE:
            SDL_Log("Mouse left window %d", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            SDL_Log("Window %d gained keyboard focus",
                    event->window.windowID);
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            SDL_Log("Window %d lost keyboard focus",
                    event->window.windowID);
            break;
        case SDL_WINDOWEVENT_CLOSE:
            SDL_Log("Window %d closed", event->window.windowID);
            break;
        default:
            SDL_Log("Window %d got unknown event %d",
                    event->window.windowID, event->window.event);
            break;
        }
    }
}
bool valInRange(int val, int min, int max)
{
	return (val >= min) && (val <= max);
}
bool rectOverlap(SDL_Rect a, SDL_Rect b)
{
	bool xOverlap = valInRange(a.x, b.x, b.x + b.w) || valInRange(b.x, a.x, a.x + a.w);
	bool yOverlap = valInRange(a.y, b.y, b.y + b.h) || valInRange(b.y, a.y, a.y + a.h);
	return xOverlap && yOverlap;
}
double getMs()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec/1000);
}
