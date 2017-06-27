#pragma once

#include <SDL/SDL.h>

#include <memory>


// Class to open an instance of an image
class image_io {
	public:
		// Create an image object
		image_io(const char* filename);
		image_io(const image_io& image_old);
		~image_io();

		SDL_Surface* get_image();

		void write(const char* filename);

		Uint32 get_pixel(int x, int y);
		void put_pixel(int x, int y, Uint32 pixel);

	private:
		SDL_Surface* m_image;
};
