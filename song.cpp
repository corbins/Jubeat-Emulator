#include <tuple>
#include <vector>
#include <stdint.h>

#include "song.h"

Song::Song() {
    perfect = 0;
    great = 0;
    good = 0;
    bad = 0;

    bpm = 0;
    beats = 4;
    note_value = 4;
    note_offset = 0;
}
