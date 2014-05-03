#include <SDL2/SDL_mixer.h>
#include <string>

#include "./audio.h"

Audio::Audio() { }

Audio::~Audio() {
    // Free the music and close the mixer.
    Mix_FreeMusic(song);
    Mix_CloseAudio();
}

void Audio::init() {
    // Initialize SDL_mixer
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
	printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }
}

void Audio::play_song(std::string const & song_title) {
    std::string song_file = std::string("data/") + song_title +
        std::string("/") + song_title + std::string(".mp3");

    song = Mix_LoadMUS(song_file.c_str());
    if(song == NULL) {
	printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
    }

    Mix_PlayMusic(song, -1);
}
