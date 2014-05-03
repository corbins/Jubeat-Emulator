#include <stdint.h>

#ifndef SRC_NOTE_H_
#define SRC_NOTE_H_

/**
 * Handles the scoring, and note accuracy information.
 */
class Note {
 public:
    uint32_t perfect;
    uint32_t great;
    uint32_t good;
    uint32_t bad;
    uint32_t miss;
    uint32_t combo;

    /**
     * Default constructor.  Initializes the scoring information to 0.
     */
    Note();
};

#endif  // SRC_NOTE_H_
