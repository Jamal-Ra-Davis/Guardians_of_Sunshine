#include <SDL2/SDL.h>
#include <stdio.h>

#include "../headers/AudioPlayer.h"

int AudioPlayer::initAudioSystem()
{
	if (SDL_Init(SDL_INIT_AUDIO))
	{
        printf("Audio init error: %s\n", SDL_GetError());
        return 1;
    }
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("Audio error: %s\n", Mix_GetError());
        return 1;
    }
	return 0;
}
int AudioPlayer::closeAudioSystem()
{
	Mix_Quit();
}
AudioPlayer::AudioPlayer(std::string s, bool music_)
{
	bgm = NULL;
	sfx = NULL;
   	music = music_; 
	if (music)
	{
		bgm = Mix_LoadMUS(s.c_str());
		if (bgm == NULL)
			printf("Could not load sound/music error: %s\n",  Mix_GetError());
	}
	else
	{
		sfx = Mix_LoadWAV(s.c_str());
		if (sfx == NULL)
			printf("Could not load sound/music error: %s\n",  Mix_GetError());
	}
}
AudioPlayer::~AudioPlayer()
{
	close();
/*
	if (bgm)
		Mix_FreeMusic(bgm);
	bgm = NULL;
	if (sfx)
		Mix_FreeChunk(sfx);
	sfx = NULL;
*/
}
void AudioPlayer::play(int repeat, int channel)
{
	if (music)
	{
		if (bgm == NULL)
			return;
		if (!Mix_PlayingMusic())
        	Mix_PlayMusic(bgm, repeat);
		else
		{
			Mix_HaltMusic();
			Mix_PlayMusic(bgm, repeat);
		}
	}
	else
	{
		if (sfx == NULL)
			return;
		Mix_PlayChannel(channel, sfx, repeat);
	}
}
void AudioPlayer::stop(int channel)
{
	if (music)
    {
		Mix_HaltMusic();
    }
    else
    {
		Mix_HaltChannel(channel);//Stops all channels
    }
}
void AudioPlayer::close()
{
	if (bgm)
    {
		Mix_FreeMusic(bgm);
		bgm = NULL;
    }
    if (sfx)
    {
    	Mix_FreeChunk(sfx);
		sfx = NULL;
    }
}
