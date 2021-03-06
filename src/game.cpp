/*
Copyright (c) 2014 Corbin Souffrant

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <SDL2/SDL.h>
#include <chrono>
#include <thread>

#include "./parser.h"
#include "./game.h"
#include "./audio.h"
#include "./input.h"
#include "./display.h"
#include "./song.h"

using std::chrono::milliseconds;

Game::Game() {
    end = false;
    render_frame = true;
    screen_width = 640;
    screen_height = 480;
    init_engine();
}

Game::~Game() {
    SDL_Quit();
}

void Game::init_engine() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error with SDL_Init\n");
    }

    audio_engine.init();
    display_engine.init(screen_width, screen_height);
    input_engine.init(&note_engine);
}

void Game::select_song() {
    // Prompt user to select a song.
    printf("Which song do you want to play?");
    while (!std::getline(std::cin, note_engine.song_title)) {
        std::cout << "which song do you want to play?";
    }

    // Parse the stepchart into the song object.
    printf("Beginning parse routine\n");
    parse_chart(note_engine.song_title, &note_engine);

    // Begin playing the song.
    printf("Beginning song\n");
    note_engine.note_index = note_engine.note_queue.begin();
    audio_engine.play_song(note_engine.song_title);
    note_engine.start_time = std::chrono::high_resolution_clock::now();
}

void Game::frame_update() {
    // Grab current time and begin processing the frame's state.
    auto cur_time = std::chrono::high_resolution_clock::now();
    note_engine.process_notes(display_engine.note_frames);

    // Rendering is done at 30FPS rather than 60FPS.
    if (render_frame) {
        display_engine.update_screen();
    }
    render_frame = !render_frame;

    // Calculate time until next frame, and sleep.
    auto sleep_time = std::chrono::duration_cast<milliseconds>
        (std::chrono::high_resolution_clock::now() - cur_time);
    std::this_thread::sleep_for(std::chrono::duration_cast<milliseconds>
                                (milliseconds(next_frame) - sleep_time));

    // Check if the round is over, either by quitting or user input.
    if (note_engine.note_index == note_engine.note_queue.end() ||
       note_engine.quit == true) {
        end = true;
    }
}

void Game::score() {
    printf("%d,%d,%d,%d,%d\n", note_engine.note_accuracy.perfect,
           note_engine.note_accuracy.great, note_engine.note_accuracy.good,
           note_engine.note_accuracy.bad, note_engine.note_accuracy.miss);
}
