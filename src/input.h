#ifndef INPUT_H
#define INPUT_H

#include "../lib/rtmidi/RtMidi.h"
#include "song.h"
#include <stdint.h>
#include <vector>

class Input {
 public:
    RtMidiIn * midi_in;
    uint32_t num_ports;

    Input();
    ~Input();

    void init(Song & song_state);
};

#endif

/*
 * The callback function for the MIDI notes, will perform the behavior
 * assigned to each note.
 * @param cur_time The current time in milliseconds of the program
 * @param message The midi byte information received
 * @param song_state The current state of the song.
 */
void process_midi(double cur_time, std::vector<unsigned char> * message,
		  void * song_state);
