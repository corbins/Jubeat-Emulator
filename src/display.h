#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>

class Display {
 public:
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Texture * marker_image;
    SDL_Texture * empty_square;

    uint32_t note_frames[4][4] = {{0}};

    Display();
    void init(uint32_t screen_width, uint32_t screen_height);
    ~Display();

    void render_empty_squares();
    void update_screen();

    /**
     * Log an SDL error with some error message to the output stream of our choice
     * @param os The output stream to write the message too
     * @param message The error message to write, format is msg error: SDL_GetError()
     */
    void logSDLError(std::ostream & os, const std::string & message);

    /**
     * Loads a BMP image into a texture on the rendering device
     * @param file The BMP image file to load
     * @return the loaded texture, or nullptr if something went wrong.
     */
    SDL_Texture * loadTexture(const std::string & file);

    /**
     * Draw an SDL_Texture to an SDL_Renderer at some destination rect
     * taking a clip of the texture if desired
     * @param texture The source texture we want to draw
     * @param dest The destination rectangle to render the texture too
     * @param clip The sub-section of the texture to draw (clipping rect)
     *		default of nullptr draws the entire texture
     */
    void renderTexture(SDL_Texture * texture, SDL_Rect dest,
		       SDL_Rect * clip);

    /*
     * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
     * the texture's width and height and taking a clip of the texture if desired
     * If a clip is passed, the clip's width and height will be used instead of
     *	the texture's
     * @param texture The source texture we want to draw
     * @param x The x coordinate to draw too
     * @param y The y coordinate to draw too
     * @param clip The sub-section of the texture to draw (clipping rect)
     *		default of nullptr draws the entire texture
     */
    void renderTexture(SDL_Texture * texture, int32_t x, int32_t y,
		       SDL_Rect * clip);

    /*
     * Draw any notes that are queued to be played.
     * @param notes The array of notes and their current frame.
     * @param image The spritesheet of the marker.
     */
    void draw_notes(SDL_Texture * image);

    /*
     * Renders the frame of the specified note.
     * @param row The row of the note to render.
     * @param col The column of the note to render.
     * @param frame The current frame of the note.
     * @param image The spritesheet of the marker.
     */
    void render_note(int32_t row, int32_t col, int32_t frame, SDL_Texture * image);
};

#endif

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
