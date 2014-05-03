/*
Copyright (c) 2014 Corbin Souffrant

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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
     * Default destructor.  Cleans up misc components.
     */
    ~Game();

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
