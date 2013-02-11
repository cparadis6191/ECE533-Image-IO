#include "image_io.h"


using namespace std;


// Parameterized constructor
// Pass it a filename to open an instance of that file
image_io::image_io(char* filename) {
	image = IMG_Load_RW(SDL_RWFromFile(filename, "rb"), 0);


	// Exit on an error
	if (!(this->image)) {
		cout << "IMG_Load_RW: " << IMG_GetError();

		exit(1);
	}


	return;
}


// Copy constructor
// Ideas taken from http://www.libsdl.org/cgi/docwiki.cgi/SDL_Surface
image_io::image_io(image_io* image_old) {
	// Copy the surface
	this->image = SDL_ConvertSurface(image_old->image,
									image_old->image->format,
									image_old->image->flags);


	return;
}


// Destructor
image_io::~image_io() {
	SDL_FreeSurface(image);


	return;
}


// Returns a pointer to the image
SDL_Surface* image_io::get_image() { return image; }


void image_io::read(const char* filename) {


	return;
}


void image_io::write(const char* filename) {
	// Returns -1 on error
	if (SDL_SaveBMP(this->image, filename)) {
		cout << "IMG_SaveBMP: " << IMG_GetError();

		exit(1);
	}


	return;
}


// Function taken from http://www.libsdl.org/cgi/docwiki.cgi/Pixel_Access
Uint32 image_io::get_pixel(int x, int y) {
	int bpp = image->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8*) image->pixels + y*image->pitch + x*bpp;

	switch(bpp) {
		case 1:
			return *p;

			break;


		case 2:
			return *(Uint16*) p;

			break;


		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return ((p[0] << 16) | (p[1] << 8) | p[2]);
			else
			return (p[0] | (p[1] << 8) | (p[2] << 16));

			break;


		case 4:
			return *(Uint32*) p;

			break;


			default:

			return 0;       /* shouldn't happen, but avoids warnings */
	}
}


// Function taken from http://www.libsdl.org/cgi/docwiki.cgi/Pixel_Access
void image_io::put_pixel(int x, int y, Uint32 pixel) {
	int bpp = image->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8*) image->pixels + y*image->pitch + x*bpp;

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
			} else {
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
