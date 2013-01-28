#ifndef _TRANSFORMS_H__
#define _TRANSFORMS_H__

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "image_io.h"


// Forward declaration
class image_io;

// gray = (0.299*r + 0.587*g + 0.114*b);
void grayscale(image_io* image_src);

// Invert color intensity
void invert(image_io* image_src);

// Apply a smoothing effect
// Utilizes a 3x3 neighborhood averaging algorithm
void smooth(image_io* image_src);

#endif
