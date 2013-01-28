#ifndef _IMAGE_IO_H__
#define _IMAGE_IO_H__

#include "transforms.h"


// gray_value = (0.299*r + 0.587*g + 0.114*b);
void RGB_to_Grayscale(image_io* image) {
	// Lock the image
	if(SDL_MUSTLOCK(image->get_image())) {
		SDL_LockSurface(image->get_image());
	}


	// Iterate through every pixel
	//image->format->BitsPerPixel
	for (int y = 0; y < image->get_image()->h; y++) {
		for (int x = 0; x < image->get_image()->w; x++) {
			Uint32 pixel_src = image->get_pixel(x, y);

			int gray_value = .299*((pixel_src >> 0) & 0xFF)
								+ .587*((pixel_src >> 8) & 0xFF)
								+ .114*((pixel_src >> 16) & 0xFF);

			Uint32 pixel_dst = (gray_value << 0)
							| (gray_value << 8)
							| (gray_value << 16);

			image->put_pixel(x, y, pixel_dst);
		}
	}


	// Unlock the image
	if(SDL_MUSTLOCK(image->get_image())) {
		SDL_UnlockSurface(image->get_image());
	}


	return;
}


void RGB_invert(image_io* image) {
	// Lock the image
	if(SDL_MUSTLOCK(image->get_image())) {
		SDL_LockSurface(image->get_image());
	}


	// Iterate through every pixel
	//image->format->BitsPerPixel
	for (int y = 0; y < image->get_image()->h; y++) {
		for (int x = 0; x < image->get_image()->w; x++) {
			Uint32 pixel_src = image->get_pixel(x, y);
			Uint32 pixel_dst = ((255 - ((pixel_src >> 0) & 0xFF)) << 0)
							| ((255 - ((pixel_src >> 8) & 0xFF)) << 8)
							| ((255 - ((pixel_src >> 16) & 0xFF)) << 16);

			image->put_pixel(x, y, pixel_dst); 
		}
	}


	// Unlock the image
	if(SDL_MUSTLOCK(image->get_image())) {
		SDL_UnlockSurface(image->get_image());
	}


	return;
}


#endif
