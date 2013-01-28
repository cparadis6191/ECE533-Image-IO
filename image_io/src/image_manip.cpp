#include "image_manip.h"


using namespace std;

int main(int argc, char* argv[]) {
	// Initialize the SDL libraries
	SDL_Init(SDL_INIT_EVERYTHING);

	image_io* image = new image_io(argv[1]);

	//grayscale(image);
	//invert(image);
	smooth(image);


	if (!SDL_SaveBMP(image->get_image(), "../inverted.bmp")) {
		cout << "IMG_SaveBMP: " << IMG_GetError();

		exit(1);
	}


	// Free memory from the images
	delete image;

	// Cleans up and closes the SDL libraries
	SDL_Quit();


	return 0;
}


