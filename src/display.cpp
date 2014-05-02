#include <fstream>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "display.h"

Display::Display() {}

void Display::init(uint32_t screen_width, uint32_t screen_height) {
    //Begin loading GUI resources
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
	logSDLError(std::cout, "SDL_Init");
    }

    //Set up the window to screen width and height.
    window = SDL_CreateWindow("Jubeat", 100, 100, screen_width,
					   screen_height, SDL_WINDOW_SHOWN);
    if (window == NULL){
	logSDLError(std::cout, "CreateWindow");
    }

    //Set up the renderer to draw to the window.
    renderer = SDL_CreateRenderer(window, -1,
						 SDL_RENDERER_ACCELERATED |
						 SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL){
	logSDLError(std::cout, "CreateRenderer");
    }

    //Load resources for the GUI
    marker_image = loadTexture("data/img/marker_sheet.bmp");
    empty_square = loadTexture("data/img/square.jpg");
}

Display::~Display() {
    if(empty_square != NULL) {
	SDL_DestroyTexture(empty_square);
    }

    if(marker_image != NULL) {
	SDL_DestroyTexture(marker_image);
    }

    if(renderer != NULL) {
	SDL_DestroyRenderer(renderer);
    }

    if(window != NULL) {
	SDL_DestroyWindow(window);
    }

    SDL_Quit();
}

void Display::logSDLError(std::ostream & os, const std::string & message) {
    os << message << " error: " << SDL_GetError() << std::endl;
}

SDL_Texture * Display::loadTexture(const std::string & file) {
    SDL_Texture * texture = IMG_LoadTexture(renderer, file.c_str());
    if (texture == NULL) {
	logSDLError(std::cout, "LoadTexture");
    }
    return texture;
}

void Display::renderTexture(SDL_Texture * texture, SDL_Rect dest,
			    SDL_Rect * clip) {
    SDL_RenderCopy(renderer, texture, clip, &dest);
}

void Display::renderTexture(SDL_Texture * texture, int32_t x, int32_t y,
			    SDL_Rect * clip) {
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    if (clip != NULL){
	dest.w = clip->w;
	dest.h = clip->h;
    }
    else
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    renderTexture(texture, dest, clip);
}

void Display::draw_notes(SDL_Texture * image) {
    int32_t cur_row = 0;
    int32_t cur_col = 0;
    for(cur_row = 0; cur_row < 4; cur_row++) {
	for(cur_col = 0; cur_col < 4; cur_col++) {
	    int32_t cur_note = note_frames[cur_row][cur_col];

	    if(cur_note != 0) {
		render_note(cur_row, cur_col, cur_note, image);
		note_frames[cur_row][cur_col] =
		    (note_frames[cur_row][cur_col] + 1) % 25;
	    }
	}
    }
}

void Display::render_note(int32_t row, int32_t col, int32_t frame,
			  SDL_Texture * image) {
    SDL_Rect clip;
    clip.x = (frame - 1) % 5 * 100;
    clip.y = (frame - 1) / 5 * 100;
    clip.w = 100;
    clip.h = 100;

    int32_t x = row * 115;
    int32_t y = col * 115;

    renderTexture(image, x, y, &clip);
}

void Display::render_empty_squares() {
    int32_t cur_row = 0;
    int32_t cur_col = 0;
    for(cur_row = 0; cur_row < 4; cur_row++) {
	for(cur_col = 0; cur_col < 4; cur_col++) {
	    int32_t x = cur_row * 115;
	    int32_t y = cur_col * 115;

	    renderTexture(empty_square, x, y, NULL);
	}
    }
}

void Display::update_screen() {
    render_empty_squares();
    draw_notes(marker_image);
    SDL_RenderPresent(renderer);
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
