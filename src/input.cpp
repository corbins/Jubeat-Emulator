/*
Copyright (c) 2014 Corbin Souffrant

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stdint.h>
#include <stdio.h>
#include <chrono>
#include <cstdlib>
#include <vector>

#include "./input.h"
#include "./song.h"
#include "../lib/rtmidi/RtMidi.h"

using std::chrono::milliseconds;

Input::Input() { }

Input::~Input() { }

void Input::init(Song * song_state) {
    // Check if there are ports available.
    midi_in = new RtMidiIn();
    num_ports = midi_in->getPortCount();
    if (num_ports == 0) {
        std::cout << "No ports available!\n";
    }

    // Connects to MPD18.
    // TODO(loliponi): Configure the port it connects to.
    //         it assumes port 1 is the correct port.
    midi_in->openPort(1);
    midi_in->setCallback(&process_midi, reinterpret_cast<void *>(song_state));
    midi_in->ignoreTypes(false, false, false);
}

void process_midi(double cur_time, std::vector<unsigned char> * message,
                         void * song_state) {
    static const int32_t BUTTON_BYTE_1 = 146;
    static const int32_t BUTTON_BYTE_2_BASE = 36;
    static const int32_t QUIT_BYTE_1 = 178;
    static const int32_t QUIT_BYTE_2 = 127;

    uint32_t num_bytes = message->size();
    Song * song = reinterpret_cast<Song *>(song_state);

    if (num_bytes >= 2 &&
        message->at(0) == BUTTON_BYTE_1) {
        int32_t note_index = message->at(1) - BUTTON_BYTE_2_BASE;
        int32_t row = 3 - note_index / 4;
        int32_t col = note_index % 4;
        uint32_t note_time = song->cur_notes[row][col];

        // Note time will only be 0 if there is no note on the screen.
        if (note_time != 0) {
            auto cur_ms_time = std::chrono::duration_cast<milliseconds>
                (std::chrono::high_resolution_clock::now() - song->start_time);

            uint32_t time_diff = cur_ms_time.count() > note_time ?
                (cur_ms_time - milliseconds(note_time)).count() :
                (milliseconds(note_time) - cur_ms_time).count();

            // Assume 60 FPS, and check timing windows, based
            //   off of frame accuracy.
            // TODO(loliponi): Figure out better timing windows.
            uint32_t frame_timing = 1000/60;

            if (time_diff < 10*frame_timing) {
                song->note_accuracy.perfect++;
                song->note_accuracy.combo++;
            } else if (time_diff < 20*frame_timing) {
                song->note_accuracy.great++;
                song->note_accuracy.combo++;
            } else if (time_diff < 30*frame_timing) {
                song->note_accuracy.good++;
                song->note_accuracy.combo = 0;
            } else {
                song->note_accuracy.bad++;
                song->note_accuracy.combo = 0;
            }

            // Prevent users from hitting same note twice.
            song->cur_notes[row][col] = 0;
            std::cout << song->note_accuracy.combo << "\n";
        }
    } else if (num_bytes >= 2 &&
               message->at(0) == QUIT_BYTE_1 &&
               message->at(2) == QUIT_BYTE_2) {
        // User hit the quit button.  Signal round to end.
        song->quit = true;
    }
}
