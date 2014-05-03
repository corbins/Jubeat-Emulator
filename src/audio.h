#include <SDL2/SDL_mixer.h>
#include <string>

#ifndef SRC_AUDIO_H_
#define SRC_AUDIO_H_

/**
 * Handles the libraries and configuration necessary for audio playback.
 * Currently using SDL mixer library, and triggers the song to play.
 */
class Audio {
 public:
    //SDL music resource
    Mix_Music * song = NULL;

    /**
     * Default constructor.  Initializes the SDL mixer library.
     */
    Audio();

    /**
     * Default destructor.  Releases the SDL mixer system.
     */
    ~Audio();

    /**
     * Initializes the SDL mixer engine.
     */
    void init();

    /**
     * Begins playing the song file when the notes begin.
     * @param song_title The name of the song to begin playing.
     */
    void play_song(const std::string & song_title);
};

#endif  // SRC_AUDIO_H_
