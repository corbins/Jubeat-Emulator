#include <stdint.h>
#include <chrono>

#include "./audio.h"
#include "./input.h"
#include "./display.h"
#include "./song.h"

#ifndef SRC_GAME_H_
#define SRC_GAME_H_

/**
 * The main Game engine.  Provides the basic functionality
 *   as well as handling the component engines.
 */
class Game {
 public:
    // True if the user wants to quit or the song finished.
    bool end;

    /**
     * Default constructor.  Initializes variables, and configures
     *   the component engines.
     */
    Game();

    /**
     * Configures the component engines to the correct states.
     */
    void init_engine();

    /**
     * Allows the user to select a song, and loads it for playback.
     */
    void select_song();

    /**
     * Called every frame to provide processing on the game's state.
     */
    void frame_update();

    /**
     * Prints out the score of the user.
     */
    void score();

 private:
    // Game configuration variables.
    const int32_t frames_per_second = 60;
    const uint32_t next_frame = 1000 / frames_per_second;
    uint32_t screen_width;
    uint32_t screen_height;
    bool render_frame;

    // Component engine objects.
    Audio audio_engine;
    Display display_engine;
    Song note_engine;
    Input input_engine;
};

#endif  // SRC_GAME_H_
