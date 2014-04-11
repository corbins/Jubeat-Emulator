#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <stdio.h>
#include <stdint.h>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include "parser.h"
#include "libs/utf8.h"
#include "song.h"

/*
 * The parsing routine.  Takes in an input of jubeat memo format (in a .txt)
 * params: The title of the file, the song object to parse into.
 */
void parse_chart(std::string & input_file);

/*
 * Adds notes into the songs buffer for timing.
 * params: The song object, Which order the notes are hit, What timing to hit at
 */
void add_notes(Song & input_song, int32_t ** note_order, int32_t * positions);

/*
 * Converts unicode characters to their correct order values.
 * NOTE: These are in UTF-8
 * params: The unicode character to convert
 * returns: The correct order representation.
 */
int32_t unicode_to_order(std::string uni_char);

/*
 * Converts header variables into the representative int.
 * params: The line to parse
 * returns: The int value, or 0 for invalid lines
 */
int32_t add_header_var(std::string line);

#endif
