#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <tuple>

#include "parser.h"
#include "song.h"


int main() {
    bool song_playing = true;

    Song example_song;
    std::string example_title("example");
    parse_chart(example_title, example_song);

    std::cout << "Beginning parse routine";

    uint32_t note_count = 0;
    std::vector<std::tuple<int32_t, int32_t, uint32_t> >::iterator notes;
    for(notes = example_song.note_position.begin(); notes != example_song.note_position.end(); ++notes, ++note_count) {
	std::cout << std::get<0>(*notes) << " " << std::get<1>(*notes) << " " << std::get<2>(*notes) << "\n";
    }

    while(song_playing) {

	//process_notes();
	break;
    }

    return 0;
}

void process_notes(std::vector<std::tuple<int32_t, int32_t> > note_queue) {
    while(note_queue.size() > 0) {
	//std::tuple<int32_t,int32_t> new_note = note_queue.pop_back();

	//hao2triggernotes
    }
}
