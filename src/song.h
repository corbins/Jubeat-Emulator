#include <chrono>
#include <tuple>
#include <vector>
#include <string>
#include <stdint.h>

#include "note.h"

#ifndef SONG_H
#define SONG_H

/**
 * The song class.  This represents the current song being played.
 *   Most values will be defined in the parser.
 */
class Song {
 public:
    //The current notes on the screen.
    uint32_t cur_notes[4][4] = {{0}};

    //The BPM and beats in the measure of the song.
    uint32_t bpm;
    uint32_t beats;

    //The number of notes per beat.
    uint32_t note_value;

    //The base offset in the beats/note_value time signature.
    uint32_t note_offset;

    //The title of the song.
    std::string song_title;

    //The time that the song started.
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();

    //The scoring and accuracy information for the song.
    Note note_accuracy;

    //Signals the game to exit the song.
    bool quit;

    //The buffer and iterator containing the note positioning.
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

#endif
