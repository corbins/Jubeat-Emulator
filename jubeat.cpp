#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <tuple>
#include <chrono>
#include <thread>
#include "lib/fmod/api/inc/fmod.hpp"
#include "lib/fmod/api/inc/fmod_errors.h"

#include "parser.h"
#include "song.h"

void process_notes(std::vector<std::tuple<int32_t, int32_t, uint32_t> > & note_queue,
		   std::chrono::milliseconds cur_time,
		   std::vector<std::tuple<int32_t, int32_t, uint32_t> >::iterator & notes);

int main() {
    //Set up timing information
    const int32_t frames_per_second = 60;
    const uint32_t next_frame = 1000 / frames_per_second;

    //Set up parsing
    Song example_song;
    std::string example_title("example");

    std::cout << "Beginning parse routine";
    parse_chart(example_title, example_song);

    uint32_t note_count = 0;
    std::vector<std::tuple<int32_t, int32_t, uint32_t> >::iterator notes;
    for(notes = example_song.note_position.begin();
	notes != example_song.note_position.end();
	++notes, ++note_count) {
	std::cout << std::get<0>(*notes) << " " << std::get<1>(*notes) <<
	    " " << std::get<2>(*notes) << "\n";
    }
    notes = example_song.note_position.begin();

    FMOD::System *system;
    FMOD::Sound *example_sound;
    FMOD::Channel *channel = 0;

    FMOD::System_Create(&system);
    system->setOutput(FMOD_OUTPUTTYPE_ALSA);
    system->init(32, FMOD_INIT_NORMAL, 0);
    system->createSound("data/example.mp3", FMOD_SOFTWARE, 0, &example_sound);

    std::chrono::high_resolution_clock::time_point start_time =
	std::chrono::high_resolution_clock::now();

    system->playSound(FMOD_CHANNEL_FREE, example_sound, 0, &channel);
    while(notes != example_song.note_position.end()) {
	std::chrono::high_resolution_clock::time_point cur_time =
	    std::chrono::high_resolution_clock::now();

	std::chrono::milliseconds cur_ms_time =
	    std::chrono::duration_cast<std::chrono::milliseconds>
	    (std::chrono::high_resolution_clock::now() - start_time);

	process_notes(example_song.note_position, cur_ms_time, notes);

	std::chrono::milliseconds next_time =
	    std::chrono::duration_cast<std::chrono::milliseconds>
	    (std::chrono::high_resolution_clock::now() -
	     cur_time);

	std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::milliseconds(next_frame) - next_time));
    }

    return 0;
}

void process_notes(std::vector<std::tuple<int32_t, int32_t, uint32_t> > & note_queue,
		   std::chrono::milliseconds cur_time,
		   std::vector<std::tuple<int32_t, int32_t, uint32_t> >::iterator & notes) {
    while(notes != note_queue.end() &&
	  std::chrono::duration_cast<std::chrono::milliseconds> (cur_time - std::chrono::milliseconds(std::get<2>(*notes))).count() >= 0) {
	std::cout << std::get<0>(*notes) << " " << std::get<1>(*notes) << "\n";
	++notes;
    }
}
