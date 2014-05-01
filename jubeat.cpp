#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <tuple>
#include <chrono>
#include <thread>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "lib/fmod/api/inc/fmod.hpp"
#include "lib/fmod/api/inc/fmod_errors.h"
#include "src/parser.h"
#include "src/song.h"

#include "lib/rtmidi/RtMidi.h"
#include <cstdlib>

//Set up namespacing to make code more readable.
using std::string;
using std::cout;
using std::vector;
using std::tuple;
using std::get;
using std::getline;
using std::chrono::milliseconds;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::this_thread::sleep_for;

/*
 * Searches the next notes to see if they are ready to be played.
 * Plays anything that is ready.
 * @param note_queue The queue of notes to search.
 * @param cur_time The current time of the song.
 * @param notes The note iterator for note_queue.
 * @param song_title The title of the song.
 * @param frames The current frame counter for the notes.
 */
void process_notes(vector<tuple<int32_t, int32_t, uint32_t> > & note_queue,
		   milliseconds cur_time,
		   vector<tuple<int32_t, int32_t, uint32_t> >::iterator & notes,
		   string & song_title,
		   int32_t frames[][4],
		   uint32_t cur_notes[][4]);

/*
 * Begins playing the song file when the notes begin.
 * @param song_title The name of the song to begin playing.
 */
void start_song(string & song_title);

/**
 * Log an SDL error with some error message to the output stream of our choice
 * @param os The output stream to write the message too
 * @param msg The error message to write, format is msg error: SDL_GetError()
 */
void logSDLError(std::ostream & os, const std::string & msg);

/**
 * Loads a BMP image into a texture on the rendering device
 * @param file The BMP image file to load
 * @param ren The renderer to load the texture onto
 * @return the loaded texture, or nullptr if something went wrong.
 */
SDL_Texture * loadTexture(const std::string & file, SDL_Renderer * ren);

/**
 * Draw an SDL_Texture to an SDL_Renderer at some destination rect
 * taking a clip of the texture if desired
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw too
 * @param dst The destination rectangle to render the texture too
 * @param clip The sub-section of the texture to draw (clipping rect)
 *		default of nullptr draws the entire texture
 */
void renderTexture(SDL_Texture * tex, SDL_Renderer * ren, SDL_Rect dst,
		   SDL_Rect * clip);

/*
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height and taking a clip of the texture if desired
 * If a clip is passed, the clip's width and height will be used instead of
 *	the texture's
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw too
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 * @param clip The sub-section of the texture to draw (clipping rect)
 *		default of nullptr draws the entire texture
 */
void renderTexture(SDL_Texture * tex, SDL_Renderer * ren, int32_t x, int32_t y,
		   SDL_Rect * clip);

/*
 * Draw any notes that are queued to be played.
 * @param notes The array of notes and their current frame.
 * @param image The spritesheet of the marker.
 * @param renderer Where to render the marker.
 */
void draw_notes(int32_t notes[][4], SDL_Texture * image,
		SDL_Renderer * renderer);

/*
 * Renders the frame of the specified note.
 * @param row The row of the note to render.
 * @param col The column of the note to render.
 * @param frame The current frame of the note.
 * @param image The spritesheet of the marker.
 * @param renderer Where to render the marker.
 */
void render_note(int32_t row, int32_t col, int32_t frame, SDL_Texture * image,
		 SDL_Renderer * renderer);

/*
 * The callback function for the MIDI notes, will perform the behavior
 * assigned to each note.
 * @param cur_time The current time in milliseconds of the program
 * @param message The midi byte information received
 * @param user_data Optional information that the callback can receive
 */
void process_midi(double cur_time, vector<unsigned char> * message,
		  void * cur_notes);

auto start_time = high_resolution_clock::now();
Song song;

int main() {
    //Set up initialization defaults
    const int32_t frames_per_second = 60;
    const uint32_t next_frame = 1000 / frames_per_second;
    const int32_t screen_width = 460;
    const int32_t screen_height = 460;
    bool render_frame = true;

    //Set up parsing

    string song_title;
    uint32_t cur_notes[4][4] = {{0}};

    cout << "Which song do you want to play?";
    while (!std::getline(std::cin, song_title)) {
	cout << "which song do you want to play?";
    }

    //Parse the stepchart into the song object.
    cout << "Beginning parse routine\n";
    parse_chart(song_title, song);

    //Begin loading GUI resources
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
	logSDLError(std::cout, "SDL_Init");
	return 1;
    }

    //Set up the window to screen width and height.
    SDL_Window * window = SDL_CreateWindow("Jubeat", 100, 100, screen_width,
					   screen_height, SDL_WINDOW_SHOWN);
    if (window == nullptr){
	logSDLError(std::cout, "CreateWindow");
	return 2;
    }

    //Set up the renderer to draw to the window.
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1,
						 SDL_RENDERER_ACCELERATED |
						 SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
	logSDLError(std::cout, "CreateRenderer");
	return 3;
    }

    //Load resources for the GUI
    SDL_Texture * marker_image = loadTexture("data/img/marker_sheet.bmp",
					   renderer);
    SDL_Texture * empty_square = loadTexture("data/img/square.jpg", renderer);

    int32_t note_frames[4][4] = {{0}};

    //Begin playing the song.
    cout << "Beginning song\n";
    auto notes = song.note_position.begin();


    RtMidiIn *midiin = new RtMidiIn();
    // Check available ports.
    uint32_t nPorts = midiin->getPortCount();
    if ( nPorts == 0 ) {
	cout << "No ports available!\n";
	return 0;
    }
    midiin->openPort( 1 );
    // Set our callback function. This should be done immediately after
    // opening the port to avoid having incoming messages written to the
    // queue.
    midiin->setCallback(&process_midi, cur_notes);
    // Don't ignore sysex, timing, or active sensing messages.
    midiin->ignoreTypes( false, false, false );

    sleep_for(milliseconds(1000));

    start_time = high_resolution_clock::now();

    start_song(song_title);

    //Loop until there are no more notes
    while(notes != song.note_position.end()) {
	auto cur_time = high_resolution_clock::now();
	auto cur_ms_time = duration_cast<milliseconds>(cur_time - start_time);

	process_notes(song.note_position, cur_ms_time, notes, song_title,
		      note_frames, cur_notes);


	if(render_frame) {
	int32_t cur_row = 0;
	int32_t cur_col = 0;
	for(cur_row = 0; cur_row < 4; cur_row++) {
	    for(cur_col = 0; cur_col < 4; cur_col++) {
		int32_t x = cur_row * 115;
		int32_t y = cur_col * 115;

		renderTexture(empty_square, renderer, x, y, NULL);
	    }
	}

	draw_notes(note_frames, marker_image, renderer);
	SDL_RenderPresent(renderer);
	}

	//Calculate time until next frame, and sleep.
	auto next_time = duration_cast<milliseconds>
	    (high_resolution_clock::now() - cur_time);

	sleep_for(duration_cast<milliseconds>
		  (milliseconds(next_frame) - next_time));
	render_frame = !render_frame;
    }

    cout << song.perfect << "," << song.great << "," << song.good << "," << song.bad << "\n";

    //Clean up GUI and return.
    SDL_DestroyTexture(empty_square);
    SDL_DestroyTexture(marker_image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void process_notes(vector<tuple<int32_t, int32_t, uint32_t> > & note_queue,
		   milliseconds cur_time,
		   vector<tuple<int32_t, int32_t, uint32_t> >::iterator & notes,
		   string & song_title,
		   int32_t frames[][4],
		   uint32_t cur_notes[][4]) {
    //Check if notes exist, and if the front note is ready to be played.
    while(notes != note_queue.end() &&
	  duration_cast<milliseconds>
	  (cur_time - milliseconds(get<2>(*notes))).count() + 40*(1000/60) >= 0) {

	//Queue the note to play on the next frame.
	int32_t row = get<0>(*notes);
	int32_t col = get<1>(*notes);

	if(cur_notes[row][col] != 0) {
	    song.miss++;
	}

	cur_notes[row][col] = get<2>(*notes);
	frames[col][row] = 1;

	++notes;
    }
}

void start_song(string & song_title) {
    FMOD::System * system;
    FMOD::Sound * sound;
    FMOD::Channel * channel = 0;
    string song_file = string("data/") + song_title + string("/") + song_title +
	string(".mp3");

    //FMod junk to start an mp3 file.
    FMOD::System_Create(& system);
    system->setOutput(FMOD_OUTPUTTYPE_ALSA);
    system->init(32, FMOD_INIT_NORMAL, 0);
    system->createSound(song_file.c_str(), FMOD_SOFTWARE, 0, & sound);
    system->playSound(FMOD_CHANNEL_FREE, sound, 0, & channel);
}

void logSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
}

SDL_Texture * loadTexture(const std::string & file, SDL_Renderer * ren){
    SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
    if (texture == nullptr) {
	logSDLError(std::cout, "LoadTexture");
    }
    return texture;
}

void renderTexture(SDL_Texture * tex, SDL_Renderer * ren, SDL_Rect dst,
		   SDL_Rect * clip = nullptr)
{
    SDL_RenderCopy(ren, tex, clip, &dst);
}

void renderTexture(SDL_Texture * tex, SDL_Renderer * ren, int32_t x, int32_t y,
	SDL_Rect * clip = nullptr)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr){
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else
	    SDL_QueryTexture(tex, NULL, NULL, & dst.w, & dst.h);

	renderTexture(tex, ren, dst, clip);
}

void draw_notes(int32_t notes[][4], SDL_Texture * image,
		SDL_Renderer * renderer) {
    int32_t cur_row = 0;
    int32_t cur_col = 0;
    for(cur_row = 0; cur_row < 4; cur_row++) {
	for(cur_col = 0; cur_col < 4; cur_col++) {
	    if(notes[cur_row][cur_col] != 0) {
		render_note(cur_row, cur_col, notes[cur_row][cur_col], image,
			    renderer);
		notes[cur_row][cur_col] = (notes[cur_row][cur_col] + 1) % 25;
	    }
	}
    }
}

void render_note(int32_t row, int32_t col, int32_t frame, SDL_Texture * image,
		 SDL_Renderer * renderer) {
    SDL_Rect clip;
    clip.x = (frame - 1) % 5 * 100;
    clip.y = (frame - 1) / 5 * 100;
    clip.w = 100;
    clip.h = 100;

    int32_t x = row * 115;
    int32_t y = col * 115;

    renderTexture(image, renderer, x, y, &clip);
}

void process_midi(double cur_time, vector<unsigned char> * message,
		  void * cur_notes) {
    uint32_t num_bytes = message->size();

    if(num_bytes >= 2 && (int)message->at(0) == 146){
	int32_t note_index = (int)message->at(1) - 36;
	int32_t row = 3 - note_index / 4;
	int32_t col = note_index % 4;
	uint32_t note_time = ((uint32_t (*)[4])cur_notes)[row][col];

	if(note_time != 0) {
	    auto cur_ms_time = duration_cast<milliseconds>
		(high_resolution_clock::now() - start_time);

	    uint32_t time_diff = cur_ms_time.count() > note_time ?
		(cur_ms_time - milliseconds(note_time)).count() :
		(milliseconds(note_time) - cur_ms_time).count();

	    if(time_diff < 10*(1000/60)) {
		song.perfect++;
		song.combo++;
	    } else if(time_diff < 20*(1000/60)) {
		song.great++;
		song.combo++;
	    } else if(time_diff < 30*(1000/60)) {
		song.good++;
		song.combo = 0;
	    } else {
		song.bad++;
		song.combo = 0;
	    }

	    cout << "Time: " << time_diff << "," << note_time << "," << cur_ms_time.count() << "\n";

	    ((uint32_t (*)[4])cur_notes)[row][col] = 0;

	    //cout << row << "," << col;
	    cout << song.perfect << "," << song.great << "," << song.good << "," << song.bad << "\n";
	    cout << song.combo << "\n";
	}
    }
}

/*
SDL code from TwinklebearDev-Lessons.
https://github.com/Twinklebear/TwinklebearDev-Lessons

License:
The MIT License (MIT)

Copyright (c) 2013 Will Usher

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
