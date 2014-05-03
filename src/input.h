/*
Copyright (c) 2014 Corbin Souffrant

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <vector>

#include "./song.h"
#include "../lib/rtmidi/RtMidi.h"

#ifndef SRC_INPUT_H_
#define SRC_INPUT_H_

/**
 * Handles the libraries and configuration for user input.
 * Currently only supports the Akai MPD18 with RtMidi library.
 */
class Input {
 public:
    // RtMidi library components.
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
    void init(Song * song_state);
};

#endif  // SRC_INPUT_H_

/**
 * The callback function for the MIDI notes, will perform the behavior
 *   assigned to each note.
 * @param cur_time The current time in milliseconds of the program.
 * @param message The midi byte information received
 * @param song_state The current state of the song.
 */
void process_midi(double cur_time, std::vector<unsigned char> * message,
                  void * song_state);
