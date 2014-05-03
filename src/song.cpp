/*
Copyright (c) 2014 Corbin Souffrant

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <tuple>
#include <vector>
#include <chrono>
#include <string>

#include "./song.h"
#include "./note.h"

using std::chrono::milliseconds;

Song::Song() {
    song_title = "";
    bpm = 0;
    beats = 4;
    note_value = 4;
    note_offset = 0;
    quit = false;
}

void Song::process_notes(uint32_t frames[][4]) {
    // Make sure something was parsed.
    if (note_queue.size() == 0) {
        return;
    }

    // Find the duration in the song.
    auto temp = std::chrono::high_resolution_clock::now();
    auto cur_dur = std::chrono::duration_cast<milliseconds>
        (temp - start_time);

    // Grab notes 60 frames in advance (At 60 fps).
    //   This is to account for notes in advance as well as
    //   delay from audio engine.
    // TODO(loliponi): Is this a constant delay?
    uint32_t frame_offset = 60 * (1000/60);

    while (note_index != note_queue.end() &&
          std::chrono::duration_cast<milliseconds>
           (cur_dur - milliseconds(std::get<2>(*note_index))).count()
           + frame_offset >= 0) {
        // Queue the note to play on the next frame.
        int32_t row = std::get<0>(*note_index);
        int32_t col = std::get<1>(*note_index);

        // Check if any of the notes haven't been cleared.
        if (cur_notes[row][col] != 0) {
            note_accuracy.miss++;
        }

        cur_notes[row][col] = std::get<2>(*note_index);
        frames[col][row] = 1;

        ++note_index;
    }
}
