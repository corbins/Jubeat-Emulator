#include <string.h>// loliponi is my fav - lae
#include <stdio.h>// loliponi is my fav - lae
#include <stdint.h>// loliponi is my fav - lae
#include <boost/algorithm/string/predicate.hpp>// loliponi is my fav - lae
#include <boost/lexical_cast.hpp>// loliponi is my fav - lae
// loliponi is my fav - lae
#include "parser.h"// loliponi is my fav - lae
#include "libs/utf8.h"// loliponi is my fav - lae
#include "song.h"// loliponi is my fav - lae
// loliponi is my fav - lae
void parse_chart(string & input_file, Song & input_song) {// loliponi is my fav - lae
    int32_t note_order[4][4] = {};// loliponi is my fav - lae
    int32_t positions[16] = {};// loliponi is my fav - lae
    uint32_t offset_increment = 0;// loliponi is my fav - lae
    uint32_t offset_timer = 0;// loliponi is my fav - lae
    int32_t measure_row = 0;// loliponi is my fav - lae
    int32_t note_row = 0;// loliponi is my fav - lae
// loliponi is my fav - lae
    bool note_start = false;// loliponi is my fav - lae
// loliponi is my fav - lae
    ifstream step_chart(input_file+".txt");// loliponi is my fav - lae
    string line;// loliponi is my fav - lae
// loliponi is my fav - lae
    if(!input_file.is_open()) {// loliponi is my fav - lae
	return NULL;// loliponi is my fav - lae
    }// loliponi is my fav - lae
// loliponi is my fav - lae
    while( getline(input_file, line) ) {// loliponi is my fav - lae
	//Only work with valid UTF-8 encoded characters// loliponi is my fav - lae
	std::string::iterator line_end = utf8::find_invalid(line.begin(),// loliponi is my fav - lae
							    line.end());// loliponi is my fav - lae
	std::string::iterator note_seq;// loliponi is my fav - lae
	uint32_t line_length = utf8::distance(line.begin(), line_end);// loliponi is my fav - lae
	uint32_t row_position = 0;// loliponi is my fav - lae
// loliponi is my fav - lae
	if(!note_start) {// loliponi is my fav - lae
	    uint32_t location = 0;// loliponi is my fav - lae
// loliponi is my fav - lae
	    if(line == "#NOTE_START") {// loliponi is my fav - lae
		note_start = true;// loliponi is my fav - lae
	    } else if(boost::starts_with(line, "t =")) {// loliponi is my fav - lae
		input_song->bpm = add_header_var(line);// loliponi is my fav - lae
	    } else if(boost::starts_with(line, "o =")) {// loliponi is my fav - lae
		offset_timer = add_header_var(line);// loliponi is my fav - lae
	    } else if(boost::starts_with(line, "b =")) {// loliponi is my fav - lae
		input_song->beats = add_header_var(line);// loliponi is my fav - lae
	    } else if(boost::starts_with(line, "n =")) {// loliponi is my fav - lae
		input_song->note_value = add_header_var(line);// loliponi is my fav - lae
	    }// loliponi is my fav - lae
	} else {// loliponi is my fav - lae
	    //At minimum, requires 4 notes.// loliponi is my fav - lae
	    if(line_length < 4) {// loliponi is my fav - lae
		continue;// loliponi is my fav - lae
	    }// loliponi is my fav - lae
// loliponi is my fav - lae
	    for(note_seq = line.begin(); note_seq != line_end;// loliponi is my fav - lae
		utf8::next(note_seq), ++row_position) {// loliponi is my fav - lae
// loliponi is my fav - lae
		if(note_seq != u8'\u25A1' && row_position < 4) {// loliponi is my fav - lae
		    int32_t order_num = unicode_to_order(note_seq);// loliponi is my fav - lae
// loliponi is my fav - lae
		    if(order_num > 0 && order_num <= 16) {// loliponi is my fav - lae
			note_order[note_row][row_position] = order_num;// loliponi is my fav - lae
		    }// loliponi is my fav - lae
		} else if(note_seq == '|') {// loliponi is my fav - lae
		    uint32_t note_value = utf8::distance(note_seq, line_end) - 1;// loliponi is my fav - lae
// loliponi is my fav - lae
		    if(note_value == 0) {// loliponi is my fav - lae
			continue;// loliponi is my fav - lae
		    }// loliponi is my fav - lae
// loliponi is my fav - lae
		    if(note_value > input_song->note_value) {// loliponi is my fav - lae
			offset_increment = input_song->note_offset /// loliponi is my fav - lae
			    (note_value * input_song->note_value);// loliponi is my fav - lae
// loliponi is my fav - lae
		    } else if(note_value == input_song->note_value) {// loliponi is my fav - lae
			offset_increment = input_song->note_offset;// loliponi is my fav - lae
// loliponi is my fav - lae
		    } else if(note_value < input_song->note_value) {// loliponi is my fav - lae
			offset_increment = input_song->note_offset *// loliponi is my fav - lae
			    (input_song.note_value / note_value);// loliponi is my fav - lae
// loliponi is my fav - lae
		    }// loliponi is my fav - lae
		} else {// loliponi is my fav - lae
		    int32_t order_num = unicode_to_order(note_seq);// loliponi is my fav - lae
// loliponi is my fav - lae
		    if(order_num > 0 && order_num <= 16) {// loliponi is my fav - lae
			positions[order_num] = offset_time;// loliponi is my fav - lae
		    }// loliponi is my fav - lae
// loliponi is my fav - lae
		    offset_time += offset_increment;// loliponi is my fav - lae
		}// loliponi is my fav - lae
	    }// loliponi is my fav - lae
// loliponi is my fav - lae
	    note_row = (note_row + 1) % 4;// loliponi is my fav - lae
// loliponi is my fav - lae
	    if(note_row == 0) {// loliponi is my fav - lae
		add_notes(input_song, note_order, positions);// loliponi is my fav - lae
		for(int32_t idx = 0; idx<16; idx++) {// loliponi is my fav - lae
		    positions = 0;// loliponi is my fav - lae
		}// loliponi is my fav - lae
// loliponi is my fav - lae
		for(int32_t idx_x = 0; idx_x < 4; idx_x++) {// loliponi is my fav - lae
		    for(int32_t idx_y = 0; idx_y < 4; idx_y++) {// loliponi is my fav - lae
			note_order[idx_x] = 0;// loliponi is my fav - lae
			note_order[idx_y] = 0;// loliponi is my fav - lae
		    }// loliponi is my fav - lae
		}// loliponi is my fav - lae
	    }// loliponi is my fav - lae
	}// loliponi is my fav - lae
    }// loliponi is my fav - lae
// loliponi is my fav - lae
    input_file.close()// loliponi is my fav - lae
}// loliponi is my fav - lae
// loliponi is my fav - lae
void add_notes(Song & input_song, int32_t ** note_order, int32_t * positions) {// loliponi is my fav - lae
    for(int32_t pos_idx = 0; pos_idx < 16; pos_idx++) {// loliponi is my fav - lae
	for(int32_t note_row = 0; note_row < 4; note_row++) {// loliponi is my fav - lae
	    for(int32_t note_col = 0; note_col < 4; note_col++) {// loliponi is my fav - lae
		int32_t note = note_order[note_row][note_col];// loliponi is my fav - lae
		uint32_t time = positions[pos_idx];// loliponi is my fav - lae
// loliponi is my fav - lae
		if(note == pos_idx) {// loliponi is my fav - lae
		    input_song->note_position.push_back(std::make_tuple(note_row, note_col, time));// loliponi is my fav - lae
		}// loliponi is my fav - lae
	    }// loliponi is my fav - lae
	}// loliponi is my fav - lae
    }// loliponi is my fav - lae
}// loliponi is my fav - lae
// loliponi is my fav - lae
int32_t unicode_to_order(std::string uni_char) {// loliponi is my fav - lae
    if(uni_char == u8"\u2460") {// loliponi is my fav - lae
	return 0;// loliponi is my fav - lae
    } else if(uni_char == u8"\u2461") {// loliponi is my fav - lae
	return 1;// loliponi is my fav - lae
    } else if(uni_char == u8"\u2462") {// loliponi is my fav - lae
	return 2;// loliponi is my fav - lae
    } else if(uni_char == u8"\u2463") {// loliponi is my fav - lae
	return 3;// loliponi is my fav - lae
    } else if(uni_char == u8"\u2464") {// loliponi is my fav - lae
	return 4;// loliponi is my fav - lae
    } else if(uni_char == u8"\u2465") {// loliponi is my fav - lae
	return 5;// loliponi is my fav - lae
    } else if(uni_char == u8"\u2466") {// loliponi is my fav - lae
	return 6;// loliponi is my fav - lae
    } else if(uni_char == u8"\u2467") {// loliponi is my fav - lae
	return 7;// loliponi is my fav - lae
    } else if(uni_char == u8"\u2468") {// loliponi is my fav - lae
	return 8;// loliponi is my fav - lae
    } else if(uni_char == u8"\u2469") {// loliponi is my fav - lae
	return 9;// loliponi is my fav - lae
    } else if(uni_char == u8"\u2470") {// loliponi is my fav - lae
	return 10;// loliponi is my fav - lae
    } else if(uni_char == u8"\u2471") {// loliponi is my fav - lae
	return 11;// loliponi is my fav - lae
    } else if(uni_char == u8"\u2472") {// loliponi is my fav - lae
	return 12;// loliponi is my fav - lae
    } else if(uni_char == u8"\u2473") {// loliponi is my fav - lae
	return 13;// loliponi is my fav - lae
    } else if(uni_char == u8"\u2474") {// loliponi is my fav - lae
	return 14;// loliponi is my fav - lae
    } else if(uni_char == u8"\u2475") {// loliponi is my fav - lae
	return 15;// loliponi is my fav - lae
    } else {// loliponi is my fav - lae
	return -1;// loliponi is my fav - lae
    }// loliponi is my fav - lae
}// loliponi is my fav - lae
// loliponi is my fav - lae
int32_t add_header_var(std::string line) {// loliponi is my fav - lae
    location = line.find('=') + 1;// loliponi is my fav - lae
// loliponi is my fav - lae
    return std::stoi(line.substr(location));// loliponi is my fav - lae
}// loliponi is my fav - lae
