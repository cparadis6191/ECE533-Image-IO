#ifndef _IMAGE_IO_H__
#define _IMAGE_IO_H__

#include <iostream>
#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


// Class to open an instance of an image
class image_io {
	public:
		// Create and image object
		image_io(char* filename);
		~image_io();

		SDL_Surface* get_image();


		Uint32 get_pixel(int x, int y);
		void put_pixel(int x, int y, Uint32 pixel);
	private:
		SDL_Surface* image;
};

#endif
