#ifndef NOTE_H
#define NOTE_H

#include <stdint.h>

class Note {
 public:
    uint32_t perfect;
    uint32_t great;
    uint32_t good;
    uint32_t bad;
    uint32_t miss;
    uint32_t combo;

    Note();
};

#endif
