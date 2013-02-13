#include "image_manip.h"

#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>


using namespace std;

int main(int argc, char** argv) {
	// Initialize command line flags
	int i_flag = 0;
	int h_flag = 0;

	// Color mask flags
	int c_flag = 0;
	int c_r_flag = 0;
	int c_g_flag = 0;
	int c_b_flag = 0;
	int c_mask = 0;
	string c_args;

	// Smooth method
	int s_med_flag = 0;
	int s_mean_flag = 0;
	string s_args;

	char* output_file = NULL;
	char* input_file = NULL;
	char c;

	opterr = 0;


	// If no command-line arguments are passed
	if (argc < 2) {
		printf("Usage: io_manip -f [INPUT]... -o [OUTPUT]... [OPTION]...\n");

		return 1;
	}


	// Parse through all the arguments
	while ((c = getopt(argc, argv, "f:o:is:hc:")) != -1) {
		switch (c) {
			// Input file
			case 'f':
				input_file = optarg;
				
				break;


			// Output file
			case 'o':
				output_file = optarg;
				
				break;


			// Invert the specified channels of the image
			case 'i':
				i_flag = 1;
				
				break;


			// Smooth the image with mean algorithm
			case 's':
				s_args = optarg;

				// Check the arguments for smoothing method
				if ((s_args.find("m") != s_args.npos)) (s_mean_flag = 1);
				if ((s_args.find("d") != s_args.npos)) (s_med_flag = 1);

				break;


			// Apply histogram equalization algorithm to the image
			case 'h':
				h_flag = 1;

				break;


			// Color mask
			case 'c':
				c_flag = 1;
				c_args = optarg;

				// Check the arguments for RGB flags and set the color mask
				if ((c_args.find("r") != c_args.npos)) (c_r_flag = 1);
				if ((c_args.find("g") != c_args.npos)) (c_g_flag = 1);
				if ((c_args.find("b") != c_args.npos)) (c_b_flag = 1);
				
				break;


			// Error checking
			case '?':
			default:
				if ((optopt == 'f') || (optopt == 'o')) {
					printf("Option -%c requires a file as an argument.\n", optopt);
				} else if ((optopt == 'c')) {
					printf("Option -%c requires an argument.\nPass the flags 'r', 'g' or 'b' to mask off those color channels.\n", optopt);
				} else if ((optopt == 's')) {
					printf("Option -%c requires an argument.\nPass the flags 'd' or 'm' to use a specific smoothing method.\n", optopt);
				} else if (isprint(optopt)) {
					printf("Unknown option '-%c'.\n", optopt);
				} else {
					printf("Unknown option character '\\x%x'.\n", optopt);
				}


				return 1;
		}
	}

	// Check for input and output files
	if (!input_file) {
		cout << "Please specify an input file!\n";

		return 1;
	}

	if (!output_file) {
		cout << "Please specify an output file!\n";

		return 1;
	}


	// Initialize the SDL libraries
	SDL_Init(SDL_INIT_EVERYTHING);


	// Open the image
	image_io* image = new image_io(input_file);


	// Compose the mask and mask off specified colors
	c_mask = (c_r_flag*M_RED | c_g_flag*M_GREEN | c_b_flag*M_BLUE);
	if (c_flag) color_mask(image, c_mask);

	// Do the the operations specified by the command line switches
	if (i_flag) invert(image);
	if (s_mean_flag) smooth_mean(image);
	if (s_med_flag) smooth_median(image);
	if (h_flag) hist_eq(image);


	// Write to a new image file
	image->write(output_file);

	// Free memory from the images
	delete image;

	// Cleans up and closes the SDL libraries
	SDL_Quit();


	return 0;
}
