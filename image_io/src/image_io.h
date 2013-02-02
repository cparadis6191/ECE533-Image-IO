#ifndef _IMAGE_IO_H__
#define _IMAGE_IO_H__

#include <iostream>
#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


// Class to open an instance of an image
class image_io {
	public:
		// Create an image object
		image_io(char* filename);
		image_io(image_io* image_new);
		~image_io(void);

		SDL_Surface* get_image(void);

		void read(const char* filename);
		void write(const char* filename);

		Uint32 get_pixel(int x, int y);
		void put_pixel(int x, int y, Uint32 pixel);

	private:
		SDL_Surface* image;
};

#endif
