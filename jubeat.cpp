#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <tuple>
#include <chrono>
#include <thread>

#include "lib/fmod/api/inc/fmod.hpp"
#include "lib/fmod/api/inc/fmod_errors.h"
#include "src/parser.h"
#include "src/song.h"

//Set up namespacing to make code more readable.
using std::string;
using std::cout;
using std::vector;
using std::tuple;
using std::get;
using std::getline;
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
		   vector<tuple<int32_t, int32_t, uint32_t> >::iterator & notes,
		   string & song_title);

/*
 * Begins playing the song file when the notes begin.
 */
void start_song(string & song_title);

int main() {
    //Set up timing information
    const int32_t frames_per_second = 60;
    const uint32_t next_frame = 1000 / frames_per_second;

    //Set up parsing
    Song song;
    string song_title;

    cout << "Which song do you want to play?";
    while (!std::getline(std::cin, song_title)) {
	cout << "which song do you want to play?";
    }

    //Parse the stepchart into the song object.
    cout << "Beginning parse routine";
    parse_chart(song_title, song);

    //Begin playing the song.
    cout << "Beginning song";
    auto notes = song.note_position.begin();
    auto start_time = high_resolution_clock::now();

    //Loop until there are no more notes
    while(notes != song.note_position.end()) {
	auto cur_time = high_resolution_clock::now();
	auto cur_ms_time = duration_cast<milliseconds>
	    (high_resolution_clock::now() - start_time);

	process_notes(song.note_position, cur_ms_time, notes, song_title);

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
		   vector<tuple<int32_t, int32_t, uint32_t> >::iterator & notes,
		   string & song_title) {
    //Check if notes exist, and if the front note is ready to be played.
    while(notes != note_queue.end() &&
	  duration_cast<milliseconds>
	  (cur_time - milliseconds(get<2>(*notes))).count() >= 0) {

	//If playing the first note, begin the song.
	if(notes == note_queue.begin()) {
	    start_song(song_title);
	}

	//Filler for now, will queue to View next week.
	cout << "Row: " << get<0>(*notes) << " Column " << get<1>(*notes) << " Note Offset: " << get<2>(*notes) << " Time: " << duration_cast<milliseconds> (cur_time - milliseconds(0)).count() << "\n";
	++notes;
    }
}

void start_song(string & song_title) {
    FMOD::System * system;
    FMOD::Sound * sound;
    FMOD::Channel * channel = 0;
    string song_file = string("data/") + song_title + string("/") + song_title +
	string(".mp3");

    //FMod junk to start an mp3 file.
    FMOD::System_Create(& system);
    system->setOutput(FMOD_OUTPUTTYPE_ALSA);
    system->init(32, FMOD_INIT_NORMAL, 0);
    system->createSound(song_file.c_str(), FMOD_SOFTWARE, 0, & sound);
    system->playSound(FMOD_CHANNEL_FREE, sound, 0, & channel);
}
