/*
Copyright (c) 2014 Corbin Souffrant

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <boost/algorithm/string/predicate.hpp>
#include <fstream>
#include <string>

#include "./parser.h"
#include "./song.h"
#include "../lib/utf8.h"

// Set up namespacing to make code more readable
using std::ifstream;
using boost::starts_with;

void parse_chart(const std::string & song_title, Song * input_song) {
    // Unicode constants
    static const int32_t USPACE      = 32;
    static const int32_t UNULL       = 0;
    static const int32_t UDASH       = 65293;
    static const int32_t USQUARE     = 9633;
    static const int32_t USQUARE_ALT = 21475;
    static const int32_t UBAR        = 124;

    // Models to hold note and timing information
    int32_t note_order[4][4] = {{0}};
    int32_t positions[16] = {0};
    uint32_t offset_increment = 0;
    uint32_t offset_timer = 0;
    int32_t note_row = 0;

    // Data to parse
    std::string song_file = std::string("data/") + song_title +
        std::string("/") + song_title + std::string(".txt");
    ifstream step_chart(song_file);
    std::string line;

    if (!step_chart.is_open()) {
        return;
    }

    while (getline(step_chart, line)) {
        // Only work with valid UTF-8 encoded characters
        std::string::iterator line_end =
            utf8::find_invalid(line.begin(), line.end());
        std::string::iterator note_seq;
        uint32_t line_length = utf8::distance(line.begin(), line_end);
        uint32_t row_position = 0;
        uint32_t cur_char = 0;
        bool note_timing = false;

        // Begin by checking the metadata for tempo, song offset, beats,
        //   and value.
        if (starts_with(line, "t =")) {
            input_song->bpm = add_header_var(line);
            if (input_song->bpm != 0 && input_song->beats != 0) {
                input_song->note_offset = ((60000 / input_song->bpm) *
                                          (input_song->note_value / 4));
            }
        } else if (starts_with(line, "o =")) {
            offset_timer += add_header_var(line);
        } else if (starts_with(line, "b =")) {
            input_song->beats = add_header_var(line);
            if (input_song->bpm != 0 && input_song->beats != 0) {
                input_song->note_offset = ((60000 / input_song->bpm) *
                                          (input_song->note_value / 4));
            }
        } else if (starts_with(line, "n =")) {
            input_song->note_value = add_header_var(line);
            if (input_song->bpm != 0 && input_song->beats != 0) {
                input_song->note_offset = ((60000 / input_song->bpm) *
                                          (input_song->note_value / 4));
            }
        } else {
            // At minimum, requires 4 notes.
            if (line_length < 4) {
                continue;
            }

            for (note_seq = line.begin(); note_seq != line_end;
                cur_char = utf8::next(note_seq, line_end)) {
                // Checks for vertical bar, signifying parsing of timing.
                if (cur_char == UBAR && note_timing == false) {
                    uint32_t note_value = utf8::distance(note_seq, line_end)-1;
                    note_timing = true;

                    if (note_value == 0) {
                        continue;
                    }

                    offset_increment = input_song->note_offset / note_value;

                  // If not in timing, then reading in note ordering
                } else if (note_timing == false) {
                    int32_t order_num = unicode_to_order(cur_char);

                    if (order_num > 0 && order_num <= 16) {
                        note_order[note_row][row_position] = order_num;
                        row_position++;
                    } else if (cur_char == USQUARE || cur_char == USQUARE_ALT) {
                        row_position++;
                    } else if (cur_char != USPACE && cur_char != UNULL) {
                        printf("Corrupted file. Bailing out: %d\n", cur_char);
                        return;
                    }
                  // In timing, assume either a note or a rest.
                } else {
                    int32_t order_num = unicode_to_order(cur_char);

                    if (order_num > 0 && order_num <= 16) {
                        positions[order_num - 1] = offset_timer;
                    } else if (cur_char != UDASH &&
                              cur_char != USPACE && cur_char != UNULL) {
                        printf("Corrupted file. Bailing out: %d\n", cur_char);
                        return;
                    }

                    offset_timer += offset_increment;
                }
            }

            // Only reads in one full 4x4 grid at a time.
            note_row = (note_row + 1) % 4;

            if (note_row == 0) {
                add_notes(input_song, note_order, positions);
                for (int32_t idx = 0; idx < 16; idx++) {
                    positions[idx] = 0;
                }

                for (int32_t idx_x = 0; idx_x < 4; idx_x++) {
                    for (int32_t idx_y = 0; idx_y < 4; idx_y++) {
                        note_order[idx_x][idx_y] = 0;
                    }
                }
            }
        }
    }
    step_chart.close();
}

void add_notes(Song * input_song,
               const int32_t note_order[][4],
               int32_t * positions) {
    for (int32_t pos_idx = 0; pos_idx < 16; pos_idx++) {
        for (int32_t note_row = 0; note_row < 4; note_row++) {
            for (int32_t note_col = 0; note_col < 4; note_col++) {
                int32_t note = note_order[note_row][note_col];
                uint32_t time = positions[pos_idx];

                if (note - 1 == pos_idx) {
                    input_song->note_queue.push_back(std::make_tuple(note_row,
                                                                       note_col,
                                                                       time));
                }
            }
        }
    }
}

int32_t unicode_to_order(const uint32_t & uni_char) {
    // Unicode value for circled 1, the rest are sequential.
    int32_t base_circled_num = 9312;

    return uni_char - base_circled_num + 1;
}

int32_t add_header_var(const std::string & line) {
    uint32_t location = line.find('=') + 1;
    try {
        return std::stoi(line.substr(location));
    } catch(...) {
        return 0;
    }
}
