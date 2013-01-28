#include "image_io.h"


using namespace std;

int main(int argc, char *argv[]) {
	// Initialize the SDL libraries
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Surface *image;
	image = IMG_Load_RW(SDL_RWFromFile(argv[1], "rb"), 0);


	// Check for an error
	if (!image) {
		cout << "IMG_Load_RW: " << IMG_GetError();
		
		return 1;
	}


	//RGB_to_Grayscale(image);
	RGB_invert(image);


	if (!SDL_SaveBMP(image, "../inverted.bmp")) {
		cout << "IMG_SaveBMP: " << IMG_GetError();

		return 1;
	}


	SDL_FreeSurface(image);

	// Cleans up and closes the SDL libraries
	SDL_Quit();


	return 0;
}


Uint32 get_pixel32(SDL_Surface* surface, int x, int y) {
	//Convert the pixels to 32 bit
	Uint32* pixels = (Uint32*) surface->pixels;


	//Get the requested pixel
	return pixels[(y*surface->w) + x];
}


// Function taken from http://www.libsdl.org/cgi/docwiki.cgi/Pixel_Access
Uint32 get_pixel(SDL_Surface *surface, int x, int y) {
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

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
void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

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
void RGB_to_Grayscale(SDL_Surface* surface) {
	// Lock the surface
	if(SDL_MUSTLOCK(surface)) {
		SDL_LockSurface(surface);
	}


	// Iterate through every pixel
	//image->format->BitsPerPixel
	for (int y = 0; y < surface->h; y++) {
		for (int x = 0; x < surface->w; x++) {
			Uint32 pixel_src = get_pixel(surface, x, y);

			int gray_value = .299*((pixel_src >> 0) & 0xFF)
								+ .587*((pixel_src >> 8) & 0xFF)
								+ .114*((pixel_src >> 16) & 0xFF);

			Uint32 pixel_dst = (gray_value << 0)
							| (gray_value << 8)
							| (gray_value << 16);

			put_pixel(surface, x, y, pixel_dst);
		}
	}


	// Unlock the surface
	if(SDL_MUSTLOCK(surface)) {
		SDL_UnlockSurface(surface);
	}


	return;
}


void RGB_invert(SDL_Surface* surface) {
	// Lock the surface
	if(SDL_MUSTLOCK(surface)) {
		SDL_LockSurface(surface);
	}


	// Iterate through every pixel
	//image->format->BitsPerPixel
	for (int y = 0; y < surface->h; y++) {
		for (int x = 0; x < surface->w; x++) {
			Uint32 pixel_src = get_pixel(surface, x, y);
			Uint32 pixel_dst = ((255 - ((pixel_src >> 0) & 0xFF)) << 0)
							| ((255 - ((pixel_src >> 8) & 0xFF)) << 8)
							| ((255 - ((pixel_src >> 16) & 0xFF)) << 16);

			put_pixel(surface, x, y, pixel_dst); 
		}
	}


	// Unlock the surface
	if(SDL_MUSTLOCK(surface)) {
		SDL_UnlockSurface(surface);
	}


	return;
}
