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

// Adjust constrast with histogram equalization algorithm
void hist_eq(image_io* image_src);

// Convert an RGB pixel representation to a grayscale value
Uint32 RGB_to_gray(Uint32 RGB_pixel);

// Convert a grayscale value back into RGB pixel representation (color is lost)
Uint32 gray_to_RGB(Uint32 gray_value);

#endif
