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

    // Grab notes 40 frames in advance (At 60 fps).
    uint32_t frame_offset = 40 * (1000/60);

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
