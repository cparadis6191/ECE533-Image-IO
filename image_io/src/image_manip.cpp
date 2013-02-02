#include "image_manip.h"


using namespace std;

int main(int argc, char* argv[]) {
	// Do some simple error checking
	if (argc != 3) {
		cout << "Program requires two arguments: i_file and o_file" << endl;

		exit(1);
	}


	// Initialize the SDL libraries
	SDL_Init(SDL_INIT_EVERYTHING);

	// Open the image
	image_io* image = new image_io(argv[1]);

	hist_eq(image);
	//grayscale(image);
	//invert(image);
	//smooth(image);

	// Write to a new image file
	image->write(argv[2]);


	// Free memory from the images
	delete image;

	// Cleans up and closes the SDL libraries
	SDL_Quit();


	return 0;
}


