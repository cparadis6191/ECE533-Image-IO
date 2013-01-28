#ifndef _TRANSFORMS_H__
#define _TRANSFORMS_H__

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


#include "image_io.h"


// Forward declaration
class image_io;

// gray = (0.299*r + 0.587*g + 0.114*b);
void RGB_to_Grayscale(image_io* image_src, image_io* image_dst);
void invert(image_io* image_src, image_io* image_dst);
void smooth(image_io* image_src, image_io* image_dst);

#endif
