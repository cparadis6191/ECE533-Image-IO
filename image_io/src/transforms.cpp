#include "transforms.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

using namespace std;

void color_mask(image_io* image_src, int c_mask) {
	// Lock the image
	if (SDL_MUSTLOCK(image_src->get_image())) {
		SDL_LockSurface(image_src->get_image());
	}


	// Holds pixel data for reading and writing
	Uint32 pixel_src, pixel_dst;

	Uint32 red_value, green_value, blue_value;

	// Iterate through every pixel
	for (int x = 0; x < image_src->get_image()->w; x++) {
		red_value = green_value = blue_value = 0;

		for (int y = 0; y < image_src->get_image()->h; y++) {
			pixel_src = image_src->get_pixel(x, y);

			// Strip colors depending on c_mask
			if (((c_mask & M_RED) != M_RED)) red_value = RGB_to_red(pixel_src);
			if (((c_mask & M_GREEN) != M_GREEN)) green_value = RGB_to_green(pixel_src);
			if (((c_mask & M_BLUE) != M_BLUE)) blue_value = RGB_to_blue(pixel_src);

			// If all colors are masked, display in grayscale
			if ((c_mask == (M_RED | M_GREEN | M_BLUE))) {
				red_value = green_value = blue_value = RGB_to_gray(pixel_src);
			}

			pixel_dst = pack_RGB(red_value, green_value, blue_value); 

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


	// Holds pixel data for reading and writing
	Uint32 pixel_src, pixel_dst;

	// Iterate through every pixel
	for (int x = 0; x < image_src->get_image()->w; x++) {
		for (int y = 0; y < image_src->get_image()->h; y++) {
			pixel_src = image_src->get_pixel(x, y);

			// Invert the color
			pixel_dst = ((255 - ((pixel_src >> 0) & 0xFF)) << 0)
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


	// Holds pixel data for reading and writing
	Uint32 pixel_src, pixel_dst;

	// Variable to hold the pixel average throughout the neighborhood
	int R_avg;
	int G_avg;
	int B_avg;

	// Iterate through every pixel, skip the outer edges
	for (int x = 1; x < image_tmp->get_image()->w - 1; x++) {
		for (int y = 1; y < image_tmp->get_image()->h - 1; y++) {
			// Variable to hold the pixel average throughout the neighborhood
			R_avg = G_avg = B_avg = 0;

			// Iterate through the neighborhood
			for (int u = -1; u + 1 < 3; u++) {
				for (int v = -1; v + 1 < 3; v++) {
					pixel_src = image_tmp->get_pixel(x + u, y + v);
					
					// Iterate through the 9 pixels in the neighborhood
					// Each has an equal weight of 1/9
					R_avg += ((pixel_src >> 0) & 0xFF);
					G_avg += ((pixel_src >> 8) & 0xFF);
					B_avg += ((pixel_src >> 16) & 0xFF);
				}
			}

			// Pack the color averages back into a single pixel
			pixel_dst = (R_avg/9 << 0)
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


	// Holds pixel data for reading and writing
	Uint32 pixel_src, pixel_dst;

	int R_med;
	int G_med;
	int B_med;

	int R_list[9];
	int G_list[9];
	int B_list[9];

	// Iterate through every pixel, skip the outer edges
	for (int x = 1; x < image_tmp->get_image()->w - 1; x++) {
		for (int y = 1; y < image_tmp->get_image()->h - 1; y++) {
			// Iterate through the neighborhood
			for (int u = -1; u + 1 < 3; u++) {
				for (int v = -1; v + 1 < 3; v++) {
					pixel_src = image_tmp->get_pixel(x + u, y + v);
					
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
			pixel_dst = (R_med << 0)
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


	// Holds pixel data for reading and writing
	Uint32 pixel_src, pixel_dst;

	Uint32 red_level_sum[256];
	Uint32 green_level_sum[256];
	Uint32 blue_level_sum[256];

	long int red_level_integral[256];
	long int green_level_integral[256];
	long int blue_level_integral[256];

	Uint8 red_value, green_value, blue_value;
	Uint32 red_value_unscaled, green_value_unscaled, blue_value_unscaled;
	Uint32 red_value_scaled, green_value_scaled, blue_value_scaled;

	for (int i = 0; i <= 255; i++) {
		red_level_sum[i] = 0;
		green_level_sum[i] = 0;
		blue_level_sum[i] = 0;

		red_level_integral[i] = 0;
		green_level_integral[i] = 0;
		blue_level_integral[i] = 0;
	}


	// Iterate through every pixel and measure the intensity
	for (int x = 0; x < image_src->get_image()->w; x++) {
		for (int y = 0; y < image_src->get_image()->h; y++) {
			pixel_src = image_src->get_pixel(x, y);

			red_value = RGB_to_red(pixel_src);
			green_value = RGB_to_green(pixel_src);
			blue_value = RGB_to_blue(pixel_src);

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
	for (int x = 0; x < image_src->get_image()->w; x++) {
		for (int y = 0; y < image_src->get_image()->h; y++) {
			pixel_src = image_src->get_pixel(x, y);

			// Separate into the red/green/blue intensities
			red_value = RGB_to_red(pixel_src);
			green_value = RGB_to_green(pixel_src);
			blue_value = RGB_to_blue(pixel_src);

			// Use the integral as the transfer function of each pixel
			red_value_unscaled = red_level_integral[red_value];
			green_value_unscaled = green_level_integral[green_value];
			blue_value_unscaled = blue_level_integral[blue_value];

			red_value_scaled = 255.0*red_value_unscaled/((double) red_level_integral[255]);
			green_value_scaled = 255.0*green_value_unscaled/((double) green_level_integral[255]);
			blue_value_scaled = 255.0*blue_value_unscaled/((double) blue_level_integral[255]);

			// Pack the color averages back into a single pixel
			pixel_dst = pack_RGB(red_value_scaled,
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

	// Holds pixel data for reading and writing
	Uint32 pixel_src, pixel_dst;
	Uint32 gray_value, bw_value;


	// Iterate through every pixel
	for (int x = 0; x < image_src->get_image()->w; x++) {
		gray_value = 0;

		for (int y = 0; y < image_src->get_image()->h; y++) {
			pixel_src = image_src->get_pixel(x, y);

			// Get the gray value of each pixel
			gray_value = RGB_to_gray(pixel_src);

			bw_value = (gray_value >= threshold)?0xFF:0x00;

			pixel_dst = pack_RGB(bw_value, bw_value, bw_value); 

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


	// Holds pixel data for reading and writing
	Uint32 pixel_src, pixel_dst;

	// Get the gray value of each pixel
	Uint32 gray_value;

	int gray_value_sum_x, gray_value_sum_y, gray_value_sum_xy;

	// Iterate through every pixel, skip the outer edges
	for (int x = 1; x < image_tmp->get_image()->w - 1; x++) {
		for (int y = 1; y < image_tmp->get_image()->h - 1; y++) {
			// Variable to hold the pixel average throughout the neighborhood
			gray_value_sum_x = gray_value_sum_y = gray_value_sum_xy = 0;

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
					pixel_src = image_tmp->get_pixel(x + u, y + v);

					// Get the gray value of each pixel
					gray_value = RGB_to_gray(pixel_src);
					
					// Iterate through the 9 pixels in the neighborhood
					// Each has a weight determined by the Sobel mask
					gray_value_sum_x += sobel_mask_x[(u + 1) + 3*(v + 1)]*gray_value;
					gray_value_sum_y += sobel_mask_y[(u + 1) + 3*(v + 1)]*gray_value;
				}
			}

			// Combine the x and y gradients with Pythagorean theorem
			gray_value_sum_xy = sqrt(pow(gray_value_sum_x, 2) + pow(gray_value_sum_y, 2));

			// Pack the color averages back into a single pixel
			pixel_dst = pack_RGB(gray_value_sum_xy, gray_value_sum_xy, gray_value_sum_xy);

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


	// Holds pixel data for reading and writing
	Uint32 pixel_src, pixel_dst;

	// Get the gray value of each pixel
	Uint32 gray_value;
	int gray_value_sum;

	// Iterate through every pixel, skip the outer edges
	for (int x = 1; x < image_tmp->get_image()->w - 1; x++) {
		for (int y = 1; y < image_tmp->get_image()->h - 1; y++) {
			// Variable to hold the pixel average throughout the neighborhood
			gray_value_sum = 0;

			// Laplace mask
			static int laplacian_mask[] = {0, 1, 0,
											1, -4, 1,
											0, 1, 0};

			// Iterate through the neighborhood
			for (int u = -1; u + 1 < 3; u++) {
				for (int v = -1; v + 1 < 3; v++) {
					pixel_src = image_tmp->get_pixel(x + u, y + v);

					// Get the gray value of each pixel
					gray_value = RGB_to_gray(pixel_src);
					
					// Iterate through the 9 pixels in the neighborhood
					gray_value_sum += laplacian_mask[(u + 1) + 3*(v + 1)]*gray_value;
				}
			}

			// Pack the color averages back into a single pixel
			pixel_dst = pack_RGB(gray_value_sum, gray_value_sum, gray_value_sum);

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


// Degrade the image by n pixels
void erosion(image_io* image_src, int erode_n) {
	// Lock the image
	if (SDL_MUSTLOCK(image_src->get_image())) {
		SDL_LockSurface(image_src->get_image());
	}


	// Holds pixel data for reading and writing
	Uint32 pixel_src, pixel_dst;
	Uint8 gray_value;

	int erode_flag;

	for (int n = 0; n < erode_n; n++) {
		// Create a copy for use in algorithms
		image_io* image_tmp = new image_io(image_src);

		// Lock the image
		if (SDL_MUSTLOCK(image_tmp->get_image())) {
			SDL_LockSurface(image_tmp->get_image());
		}


		// Iterate through every pixel, skip the outer edges
		for (int x = 1; x < image_tmp->get_image()->w - 1; x++) {
			for (int y = 1; y < image_tmp->get_image()->h - 1; y++) {
				erode_flag = 0;

				// Iterate through the neighborhood
				for (int u = -1; u + 1 < 3; u++) {
					for (int v = -1; v + 1 < 3; v++) {
						pixel_src = image_tmp->get_pixel(x + u, y + v);

						// Get the gray value of each pixel
						gray_value = RGB_to_gray(pixel_src);
						
						if (gray_value == 0xFF) {
							erode_flag = 1;
						}
					}
				}

				if (erode_flag) {
					// Pack the color averages back into a single pixel
					pixel_dst = pack_RGB(0xFF, 0xFF, 0xFF);

					image_src->put_pixel(x, y, pixel_dst); 
				}
			}
		}


		// Unlock the image
		if (SDL_MUSTLOCK(image_tmp->get_image())) {
			SDL_UnlockSurface(image_tmp->get_image());
		}

		// Free the temporary memory
		delete image_tmp;
	}


	// Unlock the image
	if (SDL_MUSTLOCK(image_src->get_image())) {
		SDL_UnlockSurface(image_src->get_image());
	}



	return;
}


// Enlarge the iamge by n pixels
// Dilates black
void dilation(image_io* image_src, int dilate_n) {
	// Lock the image
	if (SDL_MUSTLOCK(image_src->get_image())) {
		SDL_LockSurface(image_src->get_image());
	}


	// Holds pixel data for reading and writing
	Uint32 pixel_src, pixel_dst;

	Uint8 gray_value;

	for (int n = 0; n < dilate_n; n++) {
		// Create a copy for use in algorithms
		image_io* image_tmp = new image_io(image_src);

		// Lock the image
		if (SDL_MUSTLOCK(image_tmp->get_image())) {
			SDL_LockSurface(image_tmp->get_image());
		}


		// Iterate through every pixel, skip the outer edges
		for (int x = 1; x < image_tmp->get_image()->w - 1; x++) {
			for (int y = 1; y < image_tmp->get_image()->h - 1; y++) {
				pixel_src = image_tmp->get_pixel(x, y);

				// Get the gray value of each pixel
				gray_value = RGB_to_gray(pixel_src);


				// Pack the color averages back into a single pixel
				pixel_dst = pack_RGB(0x00, 0x00, 0x00);

				if (gray_value == 0x00) {
					// Iterate through the neighborhood
					for (int u = -1; u + 1 < 3; u++) {
						for (int v = -1; v + 1 < 3; v++) {
							// Fill in a 3x3 mask of pixels
							image_src->put_pixel(x + u, y + v, pixel_dst); 
						}
					}
				}
			}
		}


		// Unlock the image
		if (SDL_MUSTLOCK(image_tmp->get_image())) {
			SDL_UnlockSurface(image_tmp->get_image());
		}

		// Free the temporary memory
		delete image_tmp;
	}


	// Unlock the image
	if (SDL_MUSTLOCK(image_src->get_image())) {
		SDL_UnlockSurface(image_src->get_image());
	}


	return;
}


// Convert from RGB pixel format to gray value
// gray_value = (0.299*r + 0.587*g + 0.114*b);
Uint8 RGB_to_gray(Uint32 RGB_pixel) {
	// This math shouldn't grow greater than 255 so checking for saturation/overflow isn't necessary
	Uint32 gray_value = .3*((RGB_pixel >> 0) & 0xFF)
					+ .587*((RGB_pixel >> 8) & 0xFF)
					+ .114*((RGB_pixel >> 16) & 0xFF);


	return gray_value;
}


// Return the color components
Uint8 RGB_to_red(Uint32 RGB_pixel) { return ((RGB_pixel >> 0) & 0xFF); }
Uint8 RGB_to_green(Uint32 RGB_pixel) { return ((RGB_pixel >> 8) & 0xFF); }
Uint8 RGB_to_blue(Uint32 RGB_pixel) { return ((RGB_pixel >> 16) & 0xFF); }


// Takes red/green/blue values and packs it back into RGB representation
// Cast to a 32-bit int so the bit shifts don't shift off into nothing
// Values are passed in as 8-bit values to avoid problems where large positive numbers would cause incorrect colors to show up
Uint32 pack_RGB(Uint8 red_value, Uint8 green_value, Uint8 blue_value) {
	Uint32 RGB_pixel = (((Uint32) red_value) << 0)
					| (((Uint32) green_value) << 8)
					| (((Uint32) blue_value) << 16);


	return RGB_pixel;
}
