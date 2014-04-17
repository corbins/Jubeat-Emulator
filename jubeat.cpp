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

//Set up namespacing to make code more readable.
using std::string;
using std::cout;
using std::vector;
using std::tuple;
using std::get;
using std::chrono::milliseconds;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::this_thread::sleep_for;

/*
 * Searches the next notes to see if they are ready to be played.
 * Plays anything that is ready.
 * Params: The queue of notes, the current time, the note iterator.
 */
void process_notes(vector<tuple<int32_t, int32_t, uint32_t> > & note_queue,
		   milliseconds cur_time,
		   vector<tuple<int32_t, int32_t, uint32_t> >::iterator & notes);

/*
 * Begins playing the song file when the notes begin.
 */
void start_song();

int main() {
    //Set up timing information
    const int32_t frames_per_second = 60;
    const uint32_t next_frame = 1000 / frames_per_second;

    //Set up parsing
    Song example_song;
    string example_title("example");

    //Parse the stepchart into the song object.
    cout << "Beginning parse routine";
    parse_chart(example_title, example_song);

    //Begin playing the song.
    cout << "Beginning song";
    auto notes = example_song.note_position.begin();
    auto start_time = high_resolution_clock::now();

    //Loop until there are no more notes
    while(notes != example_song.note_position.end()) {
	auto cur_time = high_resolution_clock::now();
	auto cur_ms_time = duration_cast<milliseconds>
	    (high_resolution_clock::now() - start_time);

	process_notes(example_song.note_position, cur_ms_time, notes);

	//Calculate time until next frame, and sleep.
	auto next_time = duration_cast<milliseconds>
	    (high_resolution_clock::now() - cur_time);

	sleep_for(duration_cast<milliseconds>
		  (milliseconds(next_frame) - next_time));
    }

    return 0;
}

void process_notes(vector<tuple<int32_t, int32_t, uint32_t> > & note_queue,
		   milliseconds cur_time,
		   vector<tuple<int32_t, int32_t, uint32_t> >::iterator & notes) {
    //Check if notes exist, and if the front note is ready to be played.
    while(notes != note_queue.end() &&
	  duration_cast<milliseconds>
	  (cur_time - milliseconds(get<2>(*notes))).count() >= 0) {

	//If playing the first note, begin the song.
	if(notes == note_queue.begin()) {
	    start_song();
	}

	//Filler for now, will queue to View next week.
	cout << "Row: " << get<0>(*notes) << " Column " << get<1>(*notes) << " Note Offset: " << get<2>(*notes) << " Time: " << duration_cast<milliseconds> (cur_time - milliseconds(0)).count() << "\n";
	++notes;
    }
}

void start_song() {
    FMOD::System *system;
    FMOD::Sound *example_sound;
    FMOD::Channel *channel = 0;

    //FMod junk to start an mp3 file.
    FMOD::System_Create(&system);
    system->setOutput(FMOD_OUTPUTTYPE_ALSA);
    system->init(32, FMOD_INIT_NORMAL, 0);
    system->createSound("data/example.mp3", FMOD_SOFTWARE, 0, &example_sound);
    system->playSound(FMOD_CHANNEL_FREE, example_sound, 0, &channel);
}
