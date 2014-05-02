#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <chrono>

#include "audio.h"
#include "input.h"
#include "display.h"
#include "song.h"

class Game {
 public:
    bool end;

    Game();

    void init_engine();

    void select_song();

    void frame_update();

    void score();

 private:
    const int32_t frames_per_second = 60;
    const uint32_t next_frame = 1000 / frames_per_second;
    uint32_t screen_width;
    uint32_t screen_height;
    bool render_frame;

    Audio audio_engine;
    Display display_engine;
    Song note_engine;
    Input input_engine;
};

#endif
