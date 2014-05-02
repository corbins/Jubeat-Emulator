#include <stdint.h>
#include <stdio.h>
#include <chrono>
#include <cstdlib>
#include <vector>

#include "input.h"
#include "song.h"
#include "../lib/rtmidi/RtMidi.h"

Input::Input() { }

Input::~Input() {
    if(midi_in != NULL) {
	//delete midi_in;
    }
}

void Input::init(Song & song_state) {
    midi_in = new RtMidiIn();
    num_ports = midi_in->getPortCount();
    if (num_ports == 0) {
	std::cout << "No ports available!\n";
    }
    midi_in->openPort(1);
    midi_in->setCallback(&process_midi, (void *)&song_state);
    midi_in->ignoreTypes(false, false, false);
}

void process_midi(double cur_time, std::vector<unsigned char> * message,
			 void * song_state) {
    uint32_t num_bytes = message->size();
    Song * song = (Song *) (song_state);

    if(num_bytes >= 2 && (int)message->at(0) == 146){
	int32_t note_index = (int)message->at(1) - 36;
	int32_t row = 3 - note_index / 4;
	int32_t col = note_index % 4;
	uint32_t note_time = song->cur_notes[row][col];

	if(note_time != 0) {
	    auto cur_ms_time = std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::high_resolution_clock::now() - song->start_time);

	    uint32_t time_diff = cur_ms_time.count() > note_time ?
		(cur_ms_time - std::chrono::milliseconds(note_time)).count() :
		(std::chrono::milliseconds(note_time) - cur_ms_time).count();

	    if(time_diff < 10*(1000/60)) {
		song->note_accuracy.perfect++;
		song->note_accuracy.combo++;
	    } else if(time_diff < 20*(1000/60)) {
		song->note_accuracy.great++;
		song->note_accuracy.combo++;
	    } else if(time_diff < 30*(1000/60)) {
		song->note_accuracy.good++;
		song->note_accuracy.combo = 0;
	    } else {
		song->note_accuracy.bad++;
		song->note_accuracy.combo = 0;
	    }

	    song->cur_notes[row][col] = 0;
	    std::cout << song->note_accuracy.combo << "\n";
	}
    } else if(num_bytes >= 2 && (int)message->at(0) == 178 &&
	      (int)message->at(2) == 127) {
	song->quit = true;
    }

}
