#ifndef SONG_H
#define SONG_H

#include <tuple>
#include <vector>
#include <stdint.h>


/*
 * The song class.  This represents the current song being played.
 * Most values will be defined in the parser.
 */
class Song {
 public:
    //Default all to 0, and a 4/4 time signature.
    Song();

    //The number of perfect notes the player hit.
    uint32_t perfect;

    //The number of great notes the player hit.
    uint32_t great;

    //The number of good notes the player hit.
    uint32_t good;

    //The number of bad notes the player hit.
    uint32_t bad;

    //The combo of the player.
    uint32_t combo;

    //The BPM of the song.
    uint32_t bpm;

    //The number of beats in a measure.
    uint32_t beats;

    //The number of notes per beat.
    uint32_t note_value;

    //The base offset in the beats/note_value time signature.
    uint32_t note_offset;

    //The buffer containing the timing for all the notes.
    std::vector<std::tuple<int32_t, int32_t, uint32_t> > note_position;
};

#endif
