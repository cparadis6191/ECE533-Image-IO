#include "image_manip.h"


using namespace std;

int main(int argc, char* argv[]) {
	// Initialize the SDL libraries
	SDL_Init(SDL_INIT_EVERYTHING);

	image_io* image_src = new image_io(argv[1]);
	image_io* image_dst = new image_io(argv[1]);


	//RGB_to_Grayscale(image_src, image_dst);
	//invert(image_src, image_dst);
	smooth(image_src, image_dst);


	if (!SDL_SaveBMP(image_dst->get_image(), "../inverted.bmp")) {
		cout << "IMG_SaveBMP: " << IMG_GetError();

		exit(1);
	}

	
	// Free memory from the images
	delete image_src;
	delete image_dst;

	// Cleans up and closes the SDL libraries
	SDL_Quit();


	return 0;
}


