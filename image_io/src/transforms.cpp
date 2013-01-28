#include "transforms.h"


// gray_value = (0.299*r + 0.587*g + 0.114*b);
void grayscale(image_io* image_src) {
	// Lock the image
	if(SDL_MUSTLOCK(image_src->get_image())) {
		SDL_LockSurface(image_src->get_image());
	}


	// Iterate through every pixel
	for (int y = 0; y < image_src->get_image()->h; y++) {
		for (int x = 0; x < image_src->get_image()->w; x++) {
			Uint32 pixel_src = image_src->get_pixel(x, y);

			int gray_value = .299*((pixel_src >> 0) & 0xFF)
								+ .587*((pixel_src >> 8) & 0xFF)
								+ .114*((pixel_src >> 16) & 0xFF);

			Uint32 pixel_dst = (gray_value << 0)
							| (gray_value << 8)
							| (gray_value << 16);

			image_src->put_pixel(x, y, pixel_dst);
		}
	}


	// Unlock the image
	if(SDL_MUSTLOCK(image_src->get_image())) {
		SDL_UnlockSurface(image_src->get_image());
	}


	return;
}


void invert(image_io* image_src) {
	// Lock the image
	if(SDL_MUSTLOCK(image_src->get_image())) {
		SDL_LockSurface(image_src->get_image());
	}


	// Iterate through every pixel
	for (int y = 0; y < image_src->get_image()->h; y++) {
		for (int x = 0; x < image_src->get_image()->w; x++) {
			Uint32 pixel_src = image_src->get_pixel(x, y);
			Uint32 pixel_dst = ((255 - ((pixel_src >> 0) & 0xFF)) << 0)
							| ((255 - ((pixel_src >> 8) & 0xFF)) << 8)
							| ((255 - ((pixel_src >> 16) & 0xFF)) << 16);

			image_src->put_pixel(x, y, pixel_dst); 
		}
	}


	// Unlock the image
	if(SDL_MUSTLOCK(image_src->get_image())) {
		SDL_UnlockSurface(image_src->get_image());
	}


	return;
}


void smooth(image_io* image_src) {
	// Create a copy for use in algorithms
	image_io* image_tmp = new image_io(image_src);

	// Lock the image
	if(SDL_MUSTLOCK(image_src->get_image())) {
		SDL_LockSurface(image_src->get_image());
	}
	// Lock the image
	if(SDL_MUSTLOCK(image_tmp->get_image())) {
		SDL_LockSurface(image_tmp->get_image());
	}


	// Iterate through every pixel, skip the outer edges
	for (int y = 1; y < image_src->get_image()->h - 1; y++) {
		for (int x = 1; x < image_src->get_image()->w - 1; x++) {


			// Variable to hold the pixel average throughout the neighborhood
			int R_avg = 0;
			int G_avg = 0;
			int B_avg = 0;

			// Iterate through the neighborhood
			for (int u = -1; u + 1 < 3; u++) {
				for (int v = -1; v + 1 < 3; v++) {
					Uint32 pixel_src = image_tmp->get_pixel(x + u, y + v);
					
					// Iterate through the 9 pixels in the neighborhood
					// Each has an equal weight of 1/9
					R_avg += ((pixel_src >> 0) & 0xFF);
					G_avg += ((pixel_src >> 8) & 0xFF);
					B_avg += ((pixel_src >> 16) & 0xFF);
				}
			}

			
			// Pack the color averages back into a single pixel
			Uint32 pixel_dst = (R_avg/9 << 0)
							| (G_avg/9 << 8)
							| (B_avg/9 << 16);

			image_src->put_pixel(x, y, pixel_dst); 
		}
	}


	// Unlock the image
	if(SDL_MUSTLOCK(image_src->get_image())) {
		SDL_UnlockSurface(image_src->get_image());
	}
	// Unlock the image
	if(SDL_MUSTLOCK(image_tmp->get_image())) {
		SDL_UnlockSurface(image_tmp->get_image());
	}

	// Free the temporary memory
	delete image_tmp;


	return;
}
