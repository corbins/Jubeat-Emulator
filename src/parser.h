/*
Copyright (c) 2014 Corbin Souffrant

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <string>

#include "./song.h"

#ifndef SRC_PARSER_H_
#define SRC_PARSER_H_

/**
 * The parsing routine.  Takes in an input of jubeat memo format in a .txt.
 * @param song_title The title of the song.
 * @param input_song The song object to parse into.
 */
void parse_chart(const std::string & song_title, Song * input_song);

/**
 * Adds notes into the songs buffer for timing.
 * @param input_song The song object.
 * @param note_order The order the notes are triggered.
 * @param positions The timing for each note.
 */
void add_notes(Song * input_song, const int32_t note_order[][4],
               int32_t * positions);

/**
 * Converts UTF-8 unicode characters to their correct order values.
 * @param uni_char The character to convert.
 * @return The correct order representation. (The number inside the circle.)
 */
int32_t unicode_to_order(const uint32_t & uni_char);

/**
 * Converts header variables into the representative int.
 * @param line The line to parse.
 * @return The int value, or 0 for invalid lines.
 */
int32_t add_header_var(const std::string & line);

#endif  // SRC_PARSER_H_
