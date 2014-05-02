#include <chrono>
#include <cstdlib>
#include <vector>
#include <stdint.h>
#include <stdio.h>

#include "input.h"
#include "song.h"
#include "../lib/rtmidi/RtMidi.h"

Input::Input() { }

Input::~Input() { }

void Input::init(Song & song_state) {
    //Check if there are ports available.
    midi_in = new RtMidiIn();
    num_ports = midi_in->getPortCount();
    if (num_ports == 0) {
	std::cout << "No ports available!\n";
    }

    //Connects to MPD18.
    //TODO: Configure the port it connects to.
    //        it assumes port 1 is the correct port.
    midi_in->openPort(1);
    midi_in->setCallback(&process_midi, (void *)&song_state);
    midi_in->ignoreTypes(false, false, false);
}

void process_midi(double cur_time, std::vector<unsigned char> * message,
			 void * song_state) {
    int32_t BUTTON_BYTE_1 = 146;
    int32_t BUTTON_BYTE_2_BASE = 36;
    int32_t QUIT_BYTE_1 = 178;
    int32_t QUIT_BYTE_2 = 127;

    uint32_t num_bytes = message->size();
    Song * song = (Song *) (song_state);

    if(num_bytes >= 2 && (int)message->at(0) == BUTTON_BYTE_1){
	int32_t note_index = (int)message->at(1) - BUTTON_BYTE_2_BASE;
	int32_t row = 3 - note_index / 4;
	int32_t col = note_index % 4;
	uint32_t note_time = song->cur_notes[row][col];

	//Note time will only be 0 if there is no note on the screen.
	if(note_time != 0) {
	    auto cur_ms_time = std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::high_resolution_clock::now() - song->start_time);

	    uint32_t time_diff = cur_ms_time.count() > note_time ?
		(cur_ms_time - std::chrono::milliseconds(note_time)).count() :
		(std::chrono::milliseconds(note_time) - cur_ms_time).count();

	    //Assume 60 FPS, and check timing windows, based
	    //  off of frame accuracy.
	    //TODO: Figure out better timing windows.
	    uint32_t frame_timing = 1000/60;

	    if(time_diff < 10*frame_timing) {
		song->note_accuracy.perfect++;
		song->note_accuracy.combo++;
	    } else if(time_diff < 20*frame_timing) {
		song->note_accuracy.great++;
		song->note_accuracy.combo++;
	    } else if(time_diff < 30*frame_timing) {
		song->note_accuracy.good++;
		song->note_accuracy.combo = 0;
	    } else {
		song->note_accuracy.bad++;
		song->note_accuracy.combo = 0;
	    }

	    //Prevent users from hitting same note twice.
	    song->cur_notes[row][col] = 0;
	    std::cout << song->note_accuracy.combo << "\n";
	}
    } else if(num_bytes >= 2 && (int)message->at(0) == QUIT_BYTE_1 &&
	      (int)message->at(2) == QUIT_BYTE_2) {
	//User hit the quit button.  Signal round to end.
	song->quit = true;
    }

}
