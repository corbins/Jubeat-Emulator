#include <string>
#include <iostream>

#include "audio.h"
#include "../lib/fmod/api/inc/fmod.hpp"
#include "../lib/fmod/api/inc/fmod_errors.h"

Audio::Audio() {
    FMOD::System_Create(&system);
    system->setOutput(FMOD_OUTPUTTYPE_ALSA);
    system->init(32, FMOD_INIT_NORMAL, 0);
}

Audio::~Audio() {
    system->release();
}

void Audio::play_song(std::string song_title) {
    std::string song_file = std::string("data/") + song_title + std::string("/") +
	song_title + std::string(".mp3");

    system->createSound(song_file.c_str(), FMOD_SOFTWARE, 0, &sound);
    system->playSound(FMOD_CHANNEL_FREE, sound, 0, &channel);
}
