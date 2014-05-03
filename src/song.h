/*
Copyright (c) 2014 Corbin Souffrant

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <chrono>
#include <tuple>
#include <vector>
#include <string>

#include "./note.h"

#ifndef SRC_SONG_H_
#define SRC_SONG_H_

/**
 * The song class.  This represents the current song being played.
 *   Most values will be defined in the parser.
 */
class Song {
 public:
    // The current notes on the screen.
    uint32_t cur_notes[4][4] = {{0}};

    // The BPM and beats in the measure of the song.
    uint32_t bpm;
    uint32_t beats;

    // The number of notes per beat.
    uint32_t note_value;

    // The base offset in the beats/note_value time signature.
    uint32_t note_offset;

    // The title of the song.
    std::string song_title;

    // The time that the song started.
    std::chrono::high_resolution_clock::time_point start_time =
        std::chrono::high_resolution_clock::now();

    // The scoring and accuracy information for the song.
    Note note_accuracy;

    // Signals the game to exit the song.
    bool quit;

    // The buffer and iterator containing the note positioning.
    std::vector<std::tuple<int32_t, int32_t, uint32_t> > note_queue;
    std::vector<std::tuple<int32_t, int32_t, uint32_t> >::iterator note_index;

    /**
     * Default constructor. Inits all to 0, and a 4/4 time signature.
     */
    Song();

    /**
     * Searches the next notes to see if they are ready to be played.
     *   Plays anything that is ready.
     * @param frames The current frame counter for the notes.
     */
    void process_notes(uint32_t frames[][4]);
};

#endif  // SRC_SONG_H_
