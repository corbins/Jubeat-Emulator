/*
Copyright (c) 2014 Corbin Souffrant

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <SDL2/SDL_mixer.h>
#include <string>

#ifndef SRC_AUDIO_H_
#define SRC_AUDIO_H_

/**
 * Handles the libraries and configuration necessary for audio playback.
 * Currently using SDL mixer library, and triggers the song to play.
 */
class Audio {
 public:
    // SDL music resource.
    Mix_Music * song = NULL;

    /**
     * Default constructor.  Initializes the SDL mixer library.
     */
    Audio();

    /**
     * Default destructor.  Releases the SDL mixer system.
     */
    ~Audio();

    /**
     * Initializes the SDL mixer engine.
     */
    void init();

    /**
     * Begins playing the song file when the notes begin.
     * @param song_title The name of the song to begin playing.
     */
    void play_song(const std::string & song_title);
};

#endif  // SRC_AUDIO_H_
