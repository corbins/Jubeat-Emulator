#include <chrono>
#include <thread>
#include <stdio.h>
#include <stdint.h>

#include "parser.h"
#include "game.h"
#include "audio.h"
#include "input.h"
#include "display.h"
#include "song.h"

Game::Game() {
    end = false;
    render_frame = true;
    screen_width = 640;
    screen_height = 480;
    init_engine();
}

void Game::init_engine() {
    display_engine.init(screen_width, screen_height);
    input_engine.init(note_engine);
}

void Game::select_song() {
    //Prompt user to select a song.
    std::cout << "Which song do you want to play?";
    while (!std::getline(std::cin, note_engine.song_title)) {
	std::cout << "which song do you want to play?";
    }

    //Parse the stepchart into the song object.
    std::cout << "Beginning parse routine\n";
    parse_chart(note_engine.song_title, note_engine);

    //Begin playing the song.
    std::cout << "Beginning song\n";
    note_engine.note_index = note_engine.note_queue.begin();
    note_engine.start_time = std::chrono::high_resolution_clock::now();
    audio_engine.play_song(note_engine.song_title);
}

void Game::frame_update() {
    //Grab current time and begin processing the frame's state.
    auto cur_time = std::chrono::high_resolution_clock::now();
    note_engine.process_notes(display_engine.note_frames);

    //Rendering is done at 30FPS rather than 60FPS.
    if(render_frame) {
	display_engine.update_screen();
    }
    render_frame = !render_frame;

    //Calculate time until next frame, and sleep.
    auto sleep_time = std::chrono::duration_cast<std::chrono::milliseconds>
	(std::chrono::high_resolution_clock::now() - cur_time);
    std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::milliseconds>
				(std::chrono::milliseconds(next_frame) - sleep_time));

    //Check if the round is over, either by quitting or user input.
    if(note_engine.note_index == note_engine.note_queue.end() ||
       note_engine.quit == true) {
	end = true;
    }
}

void Game::score() {
    std::cout <<
	note_engine.note_accuracy.perfect << "," <<
	note_engine.note_accuracy.great << "," <<
	note_engine.note_accuracy.good << "," <<
	note_engine.note_accuracy.bad << "\n";
}
