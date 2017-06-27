#include "image_io.h"

#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


using namespace std;

// Parameterized constructor
// Pass it a filename to open an instance of that file
image_io::image_io(const char* filename) {
	m_image = IMG_Load_RW(SDL_RWFromFile(filename, "rb"), 0);

	// Exit on an error
	if (!m_image) {
		cout << "IMG_Load_RW: " << IMG_GetError();

		exit(1);
	}
}

// Copy constructor
// Ideas taken from http://www.libsdl.org/cgi/docwiki.cgi/SDL_Surface
image_io::image_io(const image_io& image_old) {
	// Copy the surface
	m_image = SDL_ConvertSurface(image_old.m_image,
									image_old.m_image->format,
									image_old.m_image->flags);
}

// Destructor
image_io::~image_io() {
	SDL_FreeSurface(m_image);
}

// Returns a pointer to the image
SDL_Surface* image_io::get_image() { return m_image; }

void image_io::write(const char* filename) {
	// Exits with -1 on error
	if (SDL_SaveBMP(m_image, filename)) {
		cout << "IMG_SaveBMP: " << IMG_GetError();

		exit(1);
	}
}

// Function taken from http://www.libsdl.org/cgi/docwiki.cgi/Pixel_Access
Uint32 image_io::get_pixel(int x, int y) {
	int bpp = m_image->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8*) m_image->pixels + y*m_image->pitch + x*bpp;

	switch(bpp) {
		case 1:
			return *p;
		case 2:
			return *(Uint16*) p;
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				return ((p[0] << 16) | (p[1] << 8) | p[2]);
			}
			else {
				return (p[0] | (p[1] << 8) | (p[2] << 16));
			}
		case 4:
			return *(Uint32*) p;
		default:
			return 0;	/* shouldn't happen, but avoids warnings */
	}
}

// Function taken from http://www.libsdl.org/cgi/docwiki.cgi/Pixel_Access
void image_io::put_pixel(int x, int y, Uint32 pixel) {
	int bpp = m_image->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8*) m_image->pixels + y*m_image->pitch + x*bpp;

	switch(bpp) {
		case 1:
			*p = pixel;
			break;
		case 2:
			*(Uint16*) p = pixel;
			break;
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				p[0] = (pixel >> 16) & 0xFF;
				p[1] = (pixel >> 8) & 0xFF;
				p[2] = pixel & 0xFF;
			}
			else {
				p[0] = pixel & 0xFF;
				p[1] = (pixel >> 8) & 0xFF;
				p[2] = (pixel >> 16) & 0xFF;
			}
			break;
		case 4:
			*(Uint32*) p = pixel;
			break;
	}
}
