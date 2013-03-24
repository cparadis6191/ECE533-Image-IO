#include "transforms.h"

#include <vector>
#include <algorithm>
#include <cmath>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


void color_mask(image_io* image_src, int c_mask) {
	// Lock the image
	if (SDL_MUSTLOCK(image_src->get_image())) {
		SDL_LockSurface(image_src->get_image());
	}

	Uint32 red_value, green_value, blue_value;


	// Iterate through every pixel
	for (int y = 0; y < image_src->get_image()->h; y++) {
		red_value = green_value = blue_value = 0;

		for (int x = 0; x < image_src->get_image()->w; x++) {
			Uint32 pixel_src = image_src->get_pixel(x, y);

			// Strip colors depending on c_mask
			if (((c_mask & M_RED) != M_RED)) red_value = RGB_to_red(pixel_src);
			if (((c_mask & M_GREEN) != M_GREEN)) green_value = RGB_to_green(pixel_src);
			if (((c_mask & M_BLUE) != M_BLUE)) blue_value = RGB_to_blue(pixel_src);

			// If all colors are masked, display in grayscale
			if ((c_mask == (M_RED | M_GREEN | M_BLUE))) {
				red_value = green_value = blue_value = RGB_to_gray(pixel_src);
			}

			Uint32 pixel_dst = pack_RGB(red_value, green_value, blue_value); 

			image_src->put_pixel(x, y, pixel_dst);
		}
	}


	// Unlock the image
	if (SDL_MUSTLOCK(image_src->get_image())) {
		SDL_UnlockSurface(image_src->get_image());
	}


	return;
}


void invert(image_io* image_src) {
	// Lock the image
	if (SDL_MUSTLOCK(image_src->get_image())) {
		SDL_LockSurface(image_src->get_image());
	}


	// Iterate through every pixel
	for (int y = 0; y < image_src->get_image()->h; y++) {
		for (int x = 0; x < image_src->get_image()->w; x++) {
			Uint32 pixel_src = image_src->get_pixel(x, y);

			// Invert the color
			Uint32 pixel_dst = ((255 - ((pixel_src >> 0) & 0xFF)) << 0)
							| ((255 - ((pixel_src >> 8) & 0xFF)) << 8)
							| ((255 - ((pixel_src >> 16) & 0xFF)) << 16);

			image_src->put_pixel(x, y, pixel_dst); 
		}
	}


	// Unlock the image
	if (SDL_MUSTLOCK(image_src->get_image())) {
		SDL_UnlockSurface(image_src->get_image());
	}


	return;
}


void smooth_mean(image_io* image_src) {
	// Create a copy for use in algorithms
	image_io* image_tmp = new image_io(image_src);

	// Lock the image
	if (SDL_MUSTLOCK(image_src->get_image())) {
		SDL_LockSurface(image_src->get_image());
	}
	// Lock the image
	if (SDL_MUSTLOCK(image_tmp->get_image())) {
		SDL_LockSurface(image_tmp->get_image());
	}


	// Iterate through every pixel, skip the outer edges
	for (int y = 1; y < image_tmp->get_image()->h - 1; y++) {
		for (int x = 1; x < image_tmp->get_image()->w - 1; x++) {


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
	if (SDL_MUSTLOCK(image_src->get_image())) {
		SDL_UnlockSurface(image_src->get_image());
	}
	// Unlock the image
	if (SDL_MUSTLOCK(image_tmp->get_image())) {
		SDL_UnlockSurface(image_tmp->get_image());
	}

	// Free the temporary memory
	delete image_tmp;


	return;
}


void smooth_median(image_io* image_src) {
	// Create a copy for use in algorithms
	image_io* image_tmp = new image_io(image_src);

	// Lock the image
	if (SDL_MUSTLOCK(image_src->get_image())) {
		SDL_LockSurface(image_src->get_image());
	}
	// Lock the image
	if (SDL_MUSTLOCK(image_tmp->get_image())) {
		SDL_LockSurface(image_tmp->get_image());
	}


	// Iterate through every pixel, skip the outer edges
	for (int y = 1; y < image_tmp->get_image()->h - 1; y++) {
		for (int x = 1; x < image_tmp->get_image()->w - 1; x++) {


			// Variable to hold the pixel median throughout the neighborhood
			int R_med;
			int G_med;
			int B_med;

			int R_list[9];
			int G_list[9];
			int B_list[9];

			// Iterate through the neighborhood
			for (int u = -1; u + 1 < 3; u++) {
				for (int v = -1; v + 1 < 3; v++) {
					Uint32 pixel_src = image_tmp->get_pixel(x + u, y + v);
					
					// Iterate through the 9 pixels in the neighborhood
					R_list[(u + 1) + 3*(v + 1)] = ((pixel_src >> 0) & 0xFF);
					G_list[(u + 1) + 3*(v + 1)] = ((pixel_src >> 8) & 0xFF);
					B_list[(u + 1) + 3*(v + 1)] = ((pixel_src >> 16) & 0xFF);
				}
			}

			// Sort the lists
			std::sort(R_list, R_list + 9);
			std::sort(G_list, G_list + 9);
			std::sort(B_list, B_list + 9);

			// Choose the middle value
			R_med = R_list[4];
			G_med = G_list[4];
			B_med = B_list[4];


			// Pack the color medians back into a single pixel
			Uint32 pixel_dst = (R_med << 0)
							| (G_med << 8)
							| (B_med << 16);

			image_src->put_pixel(x, y, pixel_dst); 
		}
	}


	// Unlock the image
	if (SDL_MUSTLOCK(image_src->get_image())) {
		SDL_UnlockSurface(image_src->get_image());
	}
	// Unlock the image
	if (SDL_MUSTLOCK(image_tmp->get_image())) {
		SDL_UnlockSurface(image_tmp->get_image());
	}

	// Free the temporary memory
	delete image_tmp;


	return;
}


void hist_eq(image_io* image_src) {
	// Lock the image
	if (SDL_MUSTLOCK(image_src->get_image())) {
		SDL_LockSurface(image_src->get_image());
	}


	// Keep track of RGB level intensities
	Uint32 red_level_sum[256];
	Uint32 green_level_sum[256];
	Uint32 blue_level_sum[256];
	long int red_level_integral[256];
	long int green_level_integral[256];
	long int blue_level_integral[256];

	for (int i = 0; i <= 255; i++) {
		red_level_sum[i] = 0;
		green_level_sum[i] = 0;
		blue_level_sum[i] = 0;

		red_level_integral[i] = 0;
		green_level_integral[i] = 0;
		blue_level_integral[i] = 0;
	}


	// Iterate through every pixel and measure the intensity
	for (int y = 0; y < image_src->get_image()->h; y++) {
		for (int x = 0; x < image_src->get_image()->w; x++) {
			Uint32 pixel_src = image_src->get_pixel(x, y);

			Uint8 red_value = RGB_to_red(pixel_src);
			Uint8 green_value = RGB_to_green(pixel_src);
			Uint8 blue_value = RGB_to_blue(pixel_src);

			// Increment the count of that intensity
			// This is data for the histogram
			red_level_sum[red_value] += 1;
			green_level_sum[green_value] += 1;
			blue_level_sum[blue_value] += 1;
		}
	}


	// Compute the first term of the integral
	red_level_integral[0] = red_level_sum[0];
	green_level_integral[0] = green_level_sum[0];
	blue_level_integral[0] = blue_level_sum[0];

	// Compute the integral
	for (int j = 1; j <= 255; j++) {
		// Integrate over the gray value intensity levels
		red_level_integral[j] = (red_level_sum[j] + red_level_integral[j - 1]);
		green_level_integral[j] = (green_level_sum[j] + green_level_integral[j - 1]);
		blue_level_integral[j] = (blue_level_sum[j] + blue_level_integral[j - 1]);
	}

	// Iterate through every pixel and adjust the intensity
	for (int y = 0; y < image_src->get_image()->h; y++) {
		for (int x = 0; x < image_src->get_image()->w; x++) {
			Uint32 pixel_src = image_src->get_pixel(x, y);

			// Separate into the red/green/blue intensities
			Uint8 red_value = RGB_to_red(pixel_src);
			Uint8 green_value = RGB_to_green(pixel_src);
			Uint8 blue_value = RGB_to_blue(pixel_src);

			// Use the integral as the transfer function of each pixel
			Uint32 red_value_unscaled = red_level_integral[red_value];
			Uint32 green_value_unscaled = green_level_integral[green_value];
			Uint32 blue_value_unscaled = blue_level_integral[blue_value];

			Uint32 red_value_scaled = 255.0*red_value_unscaled/((double) red_level_integral[255]);
			Uint32 green_value_scaled = 255.0*green_value_unscaled/((double) green_level_integral[255]);
			Uint32 blue_value_scaled = 255.0*blue_value_unscaled/((double) blue_level_integral[255]);

			// Pack the color averages back into a single pixel
			Uint32 pixel_dst = pack_RGB(red_value_scaled,
										green_value_scaled,
										blue_value_scaled);

			// Write to the image
			image_src->put_pixel(x, y, pixel_dst); 
		}
	}


	// Unlock the image
	if (SDL_MUSTLOCK(image_src->get_image())) {
		SDL_UnlockSurface(image_src->get_image());
	}


	return;
}


// Convert an image into a binary (black/white) image splitting at the threshold. All pixels equal to or greater than the threshold will be turned white, all pixels below will be black
void threshold(image_io* image_src, Uint32 threshold) {
	// Lock the image
	if (SDL_MUSTLOCK(image_src->get_image())) {
		SDL_LockSurface(image_src->get_image());
	}

	Uint32 gray_value, bw_value;


	// Iterate through every pixel
	for (int y = 0; y < image_src->get_image()->h; y++) {
		gray_value = 0;

		for (int x = 0; x < image_src->get_image()->w; x++) {
			Uint32 pixel_src = image_src->get_pixel(x, y);

			// Get the gray value of each pixel
			gray_value = RGB_to_gray(pixel_src);

			bw_value = (gray_value >= threshold)?0xFF:0x00;

			Uint32 pixel_dst = pack_RGB(bw_value, bw_value, bw_value); 

			image_src->put_pixel(x, y, pixel_dst);
		}
	}


	// Unlock the image
	if (SDL_MUSTLOCK(image_src->get_image())) {
		SDL_UnlockSurface(image_src->get_image());
	}


	return;
}


// Edge detection using the Sobel Gradient
void sobel_gradient(image_io* image_src) {
	// Create a copy for use in algorithms
	image_io* image_tmp = new image_io(image_src);

	// Lock the image
	if (SDL_MUSTLOCK(image_src->get_image())) {
		SDL_LockSurface(image_src->get_image());
	}
	// Lock the image
	if (SDL_MUSTLOCK(image_tmp->get_image())) {
		SDL_LockSurface(image_tmp->get_image());
	}


	// Iterate through every pixel, skip the outer edges
	for (int y = 1; y < image_tmp->get_image()->h - 1; y++) {
		for (int x = 1; x < image_tmp->get_image()->w - 1; x++) {


			// Variable to hold the pixel average throughout the neighborhood
			int gray_value_sum_x = 0;
			int gray_value_sum_y = 0;
			int gray_value_sum_xy = 0;

			// Sobel mask in the x-direction
			static int sobel_mask_x[] = {-1, 0, 1,
								-2, 0, 2,
								-1, 0, 1};

			// Sobel mask in the y-direction
			static int sobel_mask_y[] = {-1, -2, -1,
								0, 0, 0,
								1, 2, 1};

			// Iterate through the neighborhood
			for (int u = -1; u + 1 < 3; u++) {
				for (int v = -1; v + 1 < 3; v++) {
					Uint32 pixel_src = image_tmp->get_pixel(x + u, y + v);

					// Get the gray value of each pixel
					Uint32 gray_value = RGB_to_gray(pixel_src);
					
					// Iterate through the 9 pixels in the neighborhood
					// Each has a weight determined by the Sobel mask
					gray_value_sum_x += sobel_mask_x[(u + 1) + 3*(v + 1)]*gray_value;
					gray_value_sum_y += sobel_mask_y[(u + 1) + 3*(v + 1)]*gray_value;
				}
			}

			// Combine the x and y gradients with Pythagorean theorem
			gray_value_sum_xy = sqrt(pow(gray_value_sum_x, 2) + pow(gray_value_sum_y, 2));

			// Pack the color averages back into a single pixel
			Uint32 pixel_dst = pack_RGB(gray_value_sum_xy, gray_value_sum_xy, gray_value_sum_xy);

			image_src->put_pixel(x, y, pixel_dst); 
		}
	}


	// Unlock the image
	if (SDL_MUSTLOCK(image_src->get_image())) {
		SDL_UnlockSurface(image_src->get_image());
	}
	// Unlock the image
	if (SDL_MUSTLOCK(image_tmp->get_image())) {
		SDL_UnlockSurface(image_tmp->get_image());
	}

	// Free the temporary memory
	delete image_tmp;


	return;
}


// Edge detection using the Sobel Gradient
void laplacian(image_io* image_src) {
	// Create a copy for use in algorithms
	image_io* image_tmp = new image_io(image_src);

	// Lock the image
	if (SDL_MUSTLOCK(image_src->get_image())) {
		SDL_LockSurface(image_src->get_image());
	}
	// Lock the image
	if (SDL_MUSTLOCK(image_tmp->get_image())) {
		SDL_LockSurface(image_tmp->get_image());
	}


	// Iterate through every pixel, skip the outer edges
	for (int y = 1; y < image_tmp->get_image()->h - 1; y++) {
		for (int x = 1; x < image_tmp->get_image()->w - 1; x++) {


			// Variable to hold the pixel average throughout the neighborhood
			int gray_value_sum = 0;

			// Sobel mask in the x-direction
			static int laplacian_mask[] = {0, 1, 0,
											1, -4, 1,
											0, 1, 0};

			// Iterate through the neighborhood
			for (int u = -1; u + 1 < 3; u++) {
				for (int v = -1; v + 1 < 3; v++) {
					Uint32 pixel_src = image_tmp->get_pixel(x + u, y + v);

					// Get the gray value of each pixel
					Uint32 gray_value = RGB_to_gray(pixel_src);
					
					// Iterate through the 9 pixels in the neighborhood
					// Each has a weight determined by the Sobel mask
					gray_value_sum += laplacian_mask[(u + 1) + 3*(v + 1)]*gray_value;
				}
			}

			// Pack the color averages back into a single pixel
			Uint32 pixel_dst = pack_RGB(gray_value_sum, gray_value_sum, gray_value_sum);

			image_src->put_pixel(x, y, pixel_dst); 
		}
	}


	// Unlock the image
	if (SDL_MUSTLOCK(image_src->get_image())) {
		SDL_UnlockSurface(image_src->get_image());
	}
	// Unlock the image
	if (SDL_MUSTLOCK(image_tmp->get_image())) {
		SDL_UnlockSurface(image_tmp->get_image());
	}

	// Free the temporary memory
	delete image_tmp;


	return;
}



// Convert from RGB pixel format to gray value
// gray_value = (0.299*r + 0.587*g + 0.114*b);
Uint8 RGB_to_gray(Uint32 RGB_pixel) {
	// This math shouldn't grow greater than 255 so checking for saturation/overflow isn't necessary
	Uint32 gray_value = .299*((RGB_pixel >> 0) & 0xFF)
					+ .587*((RGB_pixel >> 8) & 0xFF)
					+ .114*((RGB_pixel >> 16) & 0xFF);


	return gray_value;
}


// Return the color components
Uint8 RGB_to_red(Uint32 RGB_pixel) { return ((RGB_pixel >> 0) & 0xFF); }
Uint8 RGB_to_green(Uint32 RGB_pixel) { return ((RGB_pixel >> 8) & 0xFF); }
Uint8 RGB_to_blue(Uint32 RGB_pixel) { return ((RGB_pixel >> 16) & 0xFF); }


// Takes red/green/blue values and packs it back into RGB representation
// Needs to be a 32-bit int so the bit shifts don't shift off into nothing
Uint32 pack_RGB(Uint32 red_value, Uint32 green_value, Uint32 blue_value) {
	Uint32 RGB_pixel = (red_value << 0)
					| (green_value << 8)
					| (blue_value << 16);


	return RGB_pixel;
}
