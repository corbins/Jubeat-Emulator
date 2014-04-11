#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <fstream>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include "parser.h"
#include "lib/utf8.h"
#include "song.h"

void parse_chart(std::string & input_file, Song & input_song) {
    int32_t note_order[4][4] = {{0}};
    int32_t positions[16] = {0};
    uint32_t offset_increment = 0;
    uint32_t offset_timer = 0;
    int32_t note_row = 0;

    bool note_start = false;

    std::ifstream step_chart(input_file+".txt");
    std::string line;

    if(!step_chart.is_open()) {
	return;
    }

    while( getline(step_chart, line) ) {
	//Only work with valid UTF-8 encoded characters
	std::string::iterator line_end = utf8::find_invalid(line.begin(),
							    line.end());
	std::string::iterator note_seq;
	uint32_t line_length = utf8::distance(line.begin(), line_end);
	uint32_t row_position = 0;
	uint32_t cur_char = 0;
	bool note_timing = false;

	if(!note_start) {
	    if(line == "#NOTE_START") {
		note_start = true;
	    } else if(boost::starts_with(line, "t =")) {
		input_song.bpm = add_header_var(line);
		if(input_song.bpm != 0 && input_song.beats != 0) {
		    input_song.note_offset = ((60000 / input_song.bpm) *
					      (input_song.note_value / 4));
		}
	    } else if(boost::starts_with(line, "o =")) {
		offset_timer = add_header_var(line);
	    } else if(boost::starts_with(line, "b =")) {
		input_song.beats = add_header_var(line);
		if(input_song.bpm != 0 && input_song.beats != 0) {
		    input_song.note_offset = ((60000 / input_song.bpm) *
					      (input_song.note_value / 4));
		}
	    } else if(boost::starts_with(line, "n =")) {
		input_song.note_value = add_header_var(line);
		if(input_song.bpm != 0 && input_song.beats != 0) {
		    input_song.note_offset = ((60000 / input_song.bpm) *
					      (input_song.note_value / 4));
		}

	    }
	} else {
	    //At minimum, requires 4 notes.
	    if(line_length < 4) {
		continue;
	    }

	    for(note_seq = line.begin(); note_seq != line_end;
		cur_char = utf8::next(note_seq,line_end)) {

		if(cur_char == 124) {
		    uint32_t note_value = utf8::distance(note_seq, line_end) - 1;
		    note_timing = true;

		    if(note_value == 0) {
			continue;
		    }

		    offset_increment = input_song.note_offset / note_value;
		} else if(note_timing == false) {
		    int32_t order_num = unicode_to_order(cur_char);

		    if(order_num > 0 && order_num <= 15) {
			note_order[note_row][row_position] = order_num;
			row_position++;
		    } else if(cur_char == 9633) {
			row_position++;
		    }
		} else {
		    int32_t order_num = unicode_to_order(cur_char);

		    if(order_num > 0 && order_num <= 15) {
			positions[order_num - 1] = offset_timer;
		    }

		    offset_timer += offset_increment;
		}
	    }

	    note_row = (note_row + 1) % input_song.beats;

	    if(note_row == 0) {
		add_notes(input_song, note_order, positions);
		for(int32_t idx = 0; idx<16; idx++) {
		    positions[idx] = 0;
		}

		for(int32_t idx_x = 0; idx_x < 4; idx_x++) {
		    for(int32_t idx_y = 0; idx_y < 4; idx_y++) {
			note_order[idx_x][idx_y] = 0;
		    }
		}
	    }
	}
    }
    step_chart.close();
}

void add_notes(Song & input_song, int32_t note_order[][4], int32_t * positions) {
    for(int32_t pos_idx = 0; pos_idx < 16; pos_idx++) {
	for(int32_t note_row = 0; note_row < 4; note_row++) {
	    for(int32_t note_col = 0; note_col < 4; note_col++) {
		int32_t note = note_order[note_row][note_col];
		uint32_t time = positions[pos_idx];

		if(note - 1 == pos_idx) {
		    input_song.note_position.push_back(std::make_tuple(note_row, note_col, time));
		}
	    }
	}
    }
}

int32_t unicode_to_order(uint32_t uni_char) {
    if(uni_char == 9312) {
	return 1;
    } else if(uni_char == 9313) {
	return 2;
    } else if(uni_char == 9314) {
	return 3;
    } else if(uni_char == 9315) {
	return 4;
    } else if(uni_char == 9316) {
	return 5;
    } else if(uni_char == 9317) {
	return 6;
    } else if(uni_char == 9318) {
	return 7;
    } else if(uni_char == 9319) {
	return 8;
    } else if(uni_char == 9320) {
	return 9;
    } else if(uni_char == 9321) {
	return 10;
    } else if(uni_char == 9322) {
	return 11;
    } else if(uni_char == 9323) {
	return 12;
    } else if(uni_char == 9324) {
	return 13;
    } else if(uni_char == 9325) {
	return 14;
    } else if(uni_char == 9326) {
	return 15;
    } else if(uni_char == 9327) {
	return 16;
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
