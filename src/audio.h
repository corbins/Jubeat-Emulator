#include <string>

#include "../lib/fmod/api/inc/fmod.hpp"
#include "../lib/fmod/api/inc/fmod_errors.h"

#ifndef AUDIO_H
#define AUDIO_H

/**
 * Handles the libraries and configuration necessary for audio playback.
 * Currently using FMOD library, and triggers the song to play.
 */
class Audio {
 public:
    //FMOD library components
    FMOD::System * system;
    FMOD::Sound * sound;
    FMOD::Channel * channel = 0;

    /**
     * Default constructor.  Initializes the FMOD library.
     */
    Audio();

    /**
     * Default destructor.  Releases the FMOD system.
     */
    ~Audio();

    /**
     * Begins playing the song file when the notes begin.
     * @param song_title The name of the song to begin playing.
     */
    void play_song(const std::string & song_title);
};

#endif
