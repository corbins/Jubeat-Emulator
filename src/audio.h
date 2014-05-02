#ifndef AUDIO_H
#define AUDIO_H

#include "../lib/fmod/api/inc/fmod.hpp"
#include "../lib/fmod/api/inc/fmod_errors.h"

#include <string>

class Audio {
 public:
    FMOD::System * system;
    FMOD::Sound * sound;
    FMOD::Channel * channel = 0;

    Audio();
    ~Audio();

    /*
     * Begins playing the song file when the notes begin.
     * @param song_title The name of the song to begin playing.
     */
    void play_song(std::string song_title);
};

#endif
