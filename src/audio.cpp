/*
Copyright (c) 2014 Corbin Souffrant

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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
    // Initialize SDL_mixer.
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n",
                   Mix_GetError());
    }
}

void Audio::play_song(std::string const & song_title) {
    std::string song_file = std::string("data/") + song_title +
        std::string("/") + song_title + std::string(".mp3");

    song = Mix_LoadMUS(song_file.c_str());
    if (song == NULL) {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
    }

    Mix_PlayMusic(song, -1);
}
