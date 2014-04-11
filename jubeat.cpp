#include <string.h>
#include <stdio.h>
#include <vector>
#include <tuple>

#include "parser.h"
#include "song.h"


int main() {
    Song example_song;
    std::string example_title("example");
    parse_chart(example_title, example_song);

    std::vector<std::tuple<int32_t, int32_t, uint32_t> >::iterator notes;
    for(notes = example_song.note_position.begin(); notes != example_song.note_position.end(); ++notes)
	std::cout << std::get<0>(*notes) << std::get<1>(*notes) << std::get<2>(*notes) << "\n";
    return 0;
}
