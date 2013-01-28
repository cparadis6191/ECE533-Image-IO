#include "image_manip.h"


using namespace std;

int main(int argc, char *argv[]) {
	// Initialize the SDL libraries
	SDL_Init(SDL_INIT_EVERYTHING);

	image_io* image = new image_io(argv[1]);


	image->RGB_to_Grayscale();
	image->RGB_invert();


	if (!SDL_SaveBMP(image->get_image(), "../inverted.bmp")) {
		cout << "IMG_SaveBMP: " << IMG_GetError();

		return 1;
	}

	
	delete image;

	// Cleans up and closes the SDL libraries
	SDL_Quit();


	return 0;
}


