#ifndef _TRANSFORMS_H__
#define _TRANSFORMS_H__

#include "image_io.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


// Forward declaration
class image_io;


// gray = (0.299*r + 0.587*g + 0.114*b);
void RGB_to_Grayscale(image_io* image);
void RGB_invert(image_io* image);

#endif
