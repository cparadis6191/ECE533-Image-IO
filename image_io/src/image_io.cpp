#include "image_io.h"


using namespace std;


image_io::image_io(char* filename) {
	image = IMG_Load_RW(SDL_RWFromFile(filename, "rb"), 0);


	// Check for an error
	if (!image) {
		cout << "IMG_Load_RW: " << IMG_GetError();
	}


	return;
}


image_io::~image_io() {
	SDL_FreeSurface(image);


	return;
}


// Returns a pointer to the image
SDL_Surface* image_io::get_image() { return image; }


// Function taken from http://www.libsdl.org/cgi/docwiki.cgi/Pixel_Access
Uint32 image_io::get_pixel(int x, int y) {
	int bpp = image->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)image->pixels + y * image->pitch + x * bpp;

	switch(bpp) {
		case 1:
			return *p;

			break;


		case 2:
			return *(Uint16 *)p;

			break;


		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
			else
			return p[0] | p[1] << 8 | p[2] << 16;

			break;


		case 4:
			return *(Uint32 *)p;

			break;


			default:

			return 0;       /* shouldn't happen, but avoids warnings */
	}
}


// Function taken from http://www.libsdl.org/cgi/docwiki.cgi/Pixel_Access
void image_io::put_pixel(int x, int y, Uint32 pixel) {
	int bpp = image->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)image->pixels + y * image->pitch + x * bpp;

	switch(bpp) {
		case 1:
			*p = pixel;

			break;


		case 2:
			*(Uint16 *)p = pixel;

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
			*(Uint32 *)p = pixel;

			break;
	}
}


// gray_value = (0.299*r + 0.587*g + 0.114*b);
void image_io::RGB_to_Grayscale() {
	// Lock the image
	if(SDL_MUSTLOCK(image)) {
		SDL_LockSurface(image);
	}


	// Iterate through every pixel
	//image->format->BitsPerPixel
	for (int y = 0; y < image->h; y++) {
		for (int x = 0; x < image->w; x++) {
			Uint32 pixel_src = get_pixel(x, y);

			int gray_value = .299*((pixel_src >> 0) & 0xFF)
								+ .587*((pixel_src >> 8) & 0xFF)
								+ .114*((pixel_src >> 16) & 0xFF);

			Uint32 pixel_dst = (gray_value << 0)
							| (gray_value << 8)
							| (gray_value << 16);

			put_pixel(x, y, pixel_dst);
		}
	}


	// Unlock the image
	if(SDL_MUSTLOCK(image)) {
		SDL_UnlockSurface(image);
	}


	return;
}


void image_io::RGB_invert() {
	// Lock the image
	if(SDL_MUSTLOCK(image)) {
		SDL_LockSurface(image);
	}


	// Iterate through every pixel
	//image->format->BitsPerPixel
	for (int y = 0; y < image->h; y++) {
		for (int x = 0; x < image->w; x++) {
			Uint32 pixel_src = get_pixel(x, y);
			Uint32 pixel_dst = ((255 - ((pixel_src >> 0) & 0xFF)) << 0)
							| ((255 - ((pixel_src >> 8) & 0xFF)) << 8)
							| ((255 - ((pixel_src >> 16) & 0xFF)) << 16);

			put_pixel(x, y, pixel_dst); 
		}
	}


	// Unlock the image
	if(SDL_MUSTLOCK(image)) {
		SDL_UnlockSurface(image);
	}


	return;
}
