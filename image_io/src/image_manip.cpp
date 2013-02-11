#include "image_manip.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


using namespace std;

int main(int argc, char** argv) {
	// Initialize command line flags
	int i_flag = 0;
	int g_flag = 0;
	int s_flag = 0;
	char* out_file = NULL;
	char* in_file = NULL;
	char c;

	// Parse through all the arguments
	while ((c = getopt(argc, argv, "f:o:igs")) != -1) {
		switch (c) {
			// Input file flag
			case 'f':
				in_file = optarg;
				
				break;


			// Output file flag
			case 'o':
				out_file = optarg;
				
				break;


			// Invert the image
			case 'i':
				i_flag = 1;
				
				break;


			// Convert the image to grayscale
			case 'g':
				g_flag = 1;
				
				break;


			// Smooth the image
			case 's':
				s_flag = 1;

				break;


			case '?':
			default:
				if ((optopt == 'f') || (optopt == 'o')) {
					printf("Option -%c requires an argument.\n", optopt);
				} else if (isprint(optopt)) {
					printf("Unknown option '-%c'.\n", optopt);
				} else {
					printf("Unknown option character '\\x%x'.\n", optopt);
				}


				return 1;
		}
	}


	// Initialize the SDL libraries
	SDL_Init(SDL_INIT_EVERYTHING);

	// Open the image
	image_io* image = new image_io(in_file);


	if (i_flag) invert(image);
	if (g_flag) grayscale(image);
	if (s_flag) smooth(image);
	//grayscale(image);
	//invert(image);
	//smooth(image);

	// Write to a new image file
	image->write(out_file);


	// Free memory from the images
	delete image;

	// Cleans up and closes the SDL libraries
	SDL_Quit();


	return 0;
}


