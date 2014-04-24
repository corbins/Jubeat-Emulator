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
 * Params: The queue of notes, the current time, the note iterator.
 */
void process_notes(vector<tuple<int32_t, int32_t, uint32_t> > & note_queue,
		   milliseconds cur_time,
		   vector<tuple<int32_t, int32_t, uint32_t> >::iterator & notes,
		   string & song_title);

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
 * @param row The row of the note to play.
 * @param col The column of the note to play.
 * @param notes The array of notes and their current frame.
 */
void draw_notes(int32_t row, int32_t col, int32_t * notes);

int main() {
    //Set up initialization defaults
    const int32_t frames_per_second = 60;
    const uint32_t next_frame = 1000 / frames_per_second;
    const int32_t screen_width = 800;
    const int32_t screen_height = 800;

    //Set up parsing
    Song song;
    string song_title;

    cout << "Which song do you want to play?";
    while (!std::getline(std::cin, song_title)) {
	cout << "which song do you want to play?";
    }

    //Parse the stepchart into the song object.
    cout << "Beginning parse routine";
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

    SDL_Texture * background = loadTexture("data/img/background.png", renderer);
    SDL_Rect bg_rect;
    bg_rect.x = 0;
    bg_rect.y = 0;
    bg_rect.w = 800;
    bg_rect.h = 800;

    if(background == nullptr) {
	return 4;
    }

    renderTexture(background, renderer, bg_rect, (SDL_Rect *) nullptr);
    SDL_RenderPresent(renderer);

    //Begin playing the song.
    cout << "Beginning song";
    auto notes = song.note_position.begin();
    auto start_time = high_resolution_clock::now();

    //Loop until there are no more notes
    while(notes != song.note_position.end()) {
	auto cur_time = high_resolution_clock::now();
	auto cur_ms_time = duration_cast<milliseconds>
	    (high_resolution_clock::now() - start_time);

	process_notes(song.note_position, cur_ms_time, notes, song_title);

	//Calculate time until next frame, and sleep.
	auto next_time = duration_cast<milliseconds>
	    (high_resolution_clock::now() - cur_time);

	sleep_for(duration_cast<milliseconds>
		  (milliseconds(next_frame) - next_time));
    }

    //Clean up GUI and return.
    SDL_DestroyTexture(background);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void process_notes(vector<tuple<int32_t, int32_t, uint32_t> > & note_queue,
		   milliseconds cur_time,
		   vector<tuple<int32_t, int32_t, uint32_t> >::iterator & notes,
		   string & song_title) {
    //Check if notes exist, and if the front note is ready to be played.
    while(notes != note_queue.end() &&
	  duration_cast<milliseconds>
	  (cur_time - milliseconds(get<2>(*notes))).count() >= 0) {

	//If playing the first note, begin the song.
	if(notes == note_queue.begin()) {
	    start_song(song_title);
	}

	//Filler for now, will queue to View next week.
	cout << "Row: " << get<0>(*notes) << " Column " << get<1>(*notes) << " Note Offset: " << get<2>(*notes) << " Time: " << duration_cast<milliseconds> (cur_time - milliseconds(0)).count() << "\n";
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
