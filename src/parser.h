#include <string>
#include <stdint.h>

#include "song.h"

#ifndef PARSER_H
#define PARSER_H

/**
 * The parsing routine.  Takes in an input of jubeat memo format in a .txt.
 * @param song_title The title of the song.
 * @param input_song The song object to parse into.
 */
void parse_chart(const std::string & song_title, Song & input_song);

/**
 * Adds notes into the songs buffer for timing.
 * @param input_song The song object.
 * @param note_order The order the notes are triggered.
 * @param positions The timing for each note.
 */
void add_notes(Song & input_song, const int32_t note_order[][4], int32_t * positions);

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

#endif
