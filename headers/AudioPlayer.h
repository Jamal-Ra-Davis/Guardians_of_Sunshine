#pragma once
#include <SDL2/SDL_mixer.h>
#include <string>

class AudioPlayer
{
	private:
		Mix_Music *bgm;
    	Mix_Chunk *sfx;
		bool music;
	public:
		static const int LOOP = -1;
		static int initAudioSystem();
		static int closeAudioSystem();
		AudioPlayer(std::string s, bool music_);
		~AudioPlayer();
		void play(int repeat, int channel=-1);
		void stop(int channel=-1);
		void close();
		bool isPlaying() {return Mix_PlayingMusic();};
};
