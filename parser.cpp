#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include "parser.h"
#include "lib/utf8.h"
#include "song.h"

void parse_chart(std::string & input_file, Song & input_song) {
    int32_t note_order[4][4] = {};
    int32_t positions[16] = {};
    uint32_t offset_increment = 0;
    uint32_t offset_timer = 0;
    int32_t measure_row = 0;
    int32_t note_row = 0;

    bool note_start = false;

    ifstream step_chart(input_file+".txt");
    string line;

    if(!input_file.is_open()) {
	return NULL;
    }

    while( getline(input_file, line) ) {
	//Only work with valid UTF-8 encoded characters
	std::string::iterator line_end = utf8::find_invalid(line.begin(),
							    line.end());
	std::string note_seq::iterator;
	uint32_t line_length = utf8::distance(line.begin(), line_end);
	uint32_t row_position = 0;

	if(!note_start) {
	    if(line == "#NOTE_START") {
		note_start = true;
	    } else if(boost::starts_with(line, "t =")) {
		input_song->bpm = add_header_var(line);
	    } else if(boost::starts_with(line, "o =")) {
		offset_timer = add_header_var(line);
	    } else if(boost::starts_with(line, "b =")) {
		input_song->beats = add_header_var(line);
	    } else if(boost::starts_with(line, "n =")) {
		input_song->note_value = add_header_var(line);
	    }
	} else {
	    //At minimum, requires 4 notes.
	    if(line_length < 4) {
		continue;
	    }

	    for(note_seq = line.begin(); note_seq != line_end;
		utf8::next(note_seq), ++row_position) {

		if(std::strcmp(note_seq, "\u25A1") != 0 && row_position < 4) {
		    int32_t order_num = unicode_to_order(note_seq);

		    if(order_num > 0 && order_num <= 16) {
			note_order[note_row][row_position] = order_num;
		    }
		} else if(std::strcmp(note_seq, "|")) {
		    uint32_t note_value = utf8::distance(note_seq, line_end) - 1;

		    if(note_value == 0) {
			continue;
		    }

		    if(note_value > input_song->note_value) {
			offset_increment = input_song->note_offset /
			    (note_value * input_song->note_value);

		    } else if(note_value == input_song->note_value) {
			offset_increment = input_song->note_offset;

		    } else if(note_value < input_song->note_value) {
			offset_increment = input_song->note_offset *
			    (input_song.note_value / note_value);

		    }
		} else {
		    int32_t order_num = unicode_to_order(note_seq);

		    if(order_num > 0 && order_num <= 16) {
			positions[order_num] = offset_time;
		    }

		    offset_time += offset_increment;
		}
	    }

	    note_row = (note_row + 1) % 4;

	    if(note_row == 0) {
		add_notes(input_song, note_order, positions);
		for(int32_t idx = 0; idx<16; idx++) {
		    positions = 0;
		}

		for(int32_t idx_x = 0; idx_x < 4; idx_x++) {
		    for(int32_t idx_y = 0; idx_y < 4; idx_y++) {
			note_order[idx_x] = 0;
			note_order[idx_y] = 0;
		    }
		}
	    }
	}
    }
    step_chart.close()
}

void add_notes(Song & input_song, int32_t ** note_order, int32_t * positions) {
    for(int32_t pos_idx = 0; pos_idx < 16; pos_idx++) {
	for(int32_t note_row = 0; note_row < 4; note_row++) {
	    for(int32_t note_col = 0; note_col < 4; note_col++) {
		int32_t note = note_order[note_row][note_col];
		uint32_t time = positions[pos_idx];

		if(note == pos_idx) {
		    input_song.note_position.push_back(std::make_tuple(note_row, note_col, time));
		}
	    }
	}
    }
}

int32_t unicode_to_order(std::string uni_char) {
    if(uni_char == "\u2460") {
	return 0;
    } else if(uni_char == "\u2461") {
	return 1;
    } else if(uni_char == u8"\u2462") {
	return 2;
    } else if(uni_char == u8"\u2463") {
	return 3;
    } else if(uni_char == u8"\u2464") {
	return 4;
    } else if(uni_char == u8"\u2465") {
	return 5;
    } else if(uni_char == u8"\u2466") {
	return 6;
    } else if(uni_char == u8"\u2467") {
	return 7;
    } else if(uni_char == u8"\u2468") {
	return 8;
    } else if(uni_char == u8"\u2469") {
	return 9;
    } else if(uni_char == u8"\u2470") {
	return 10;
    } else if(uni_char == u8"\u2471") {
	return 11;
    } else if(uni_char == u8"\u2472") {
	return 12;
    } else if(uni_char == u8"\u2473") {
	return 13;
    } else if(uni_char == u8"\u2474") {
	return 14;
    } else if(uni_char == u8"\u2475") {
	return 15;
    }

    return -1;
}

int32_t add_header_var(std::string line) {
    uint32_t location = line.find('=') + 1;
    try {
	return std::stoi(line.substr(location));
    } catch(...) {
	return 0;
    }
}
