#include <vector>
#include <stdint.h>

#include "song.h"
#include "../lib/rtmidi/RtMidi.h"

#ifndef INPUT_H
#define INPUT_H

/**
 * Handles the libraries and configuration for user input.
 * Currently only supports the Akai MPD18 with RtMidi library.
 */
class Input {
 public:
    //RtMidi library components.
    RtMidiIn * midi_in;
    uint32_t num_ports;

    /**
     * Default constructor.
     */
    Input();

    /**
     * Default destructor.
     */
    ~Input();

    /**
     * Initializes the input component, and configures it to
     *   interact with the song state engine.
     * @param song_state The song state engine to interact with.
     */
    void init(Song & song_state);
};

#endif

/**
 * The callback function for the MIDI notes, will perform the behavior
 *   assigned to each note.
 * @param cur_time The current time in milliseconds of the program.
 * @param message The midi byte information received
 * @param song_state The current state of the song.
 */
void process_midi(double cur_time, std::vector<unsigned char> * message,
		  void * song_state);
