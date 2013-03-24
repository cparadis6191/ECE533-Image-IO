#ifndef _TRANSFORMS_H__
#define _TRANSFORMS_H__

#include "image_io.h"

// Color mask flags
#define M_RED (1 << 0)
#define M_GREEN (1 << 1)
#define M_BLUE (1 << 2)


// Forward declaration
class image_io;

// Choose a color to mask off
// gray = (0.299*r + 0.587*g + 0.114*b);
void color_mask(image_io* image_src, int mask);

// Invert color intensity
void invert(image_io* image_src);

// Apply a smoothing effect
// Utilizes a 3x3 neighborhood averaging algorithm
void smooth_mean(image_io* image_src);
// Utilizes a 3x3 neighborhood median algorithm
void smooth_median(image_io* image_src);

// Adjust constrast with histogram equalization algorithm
void hist_eq(image_io* image_src);

// Convert an image into a binary (black/white) image splitting at the threshold. All pixels equal to or greater than the threshold will be turned white, all pixels below will be black
void threshold(image_io* image_src, Uint32 threshold);

// Edge detection using the Sobel Gradient
void sobel_gradient(image_io* image_src);


void laplacian(image_io* image_src);


void erosion(image_io* image_src);


void dilation(image_io* image_src);

// Convert an RGB pixel representation to a grayscale value
Uint8 RGB_to_gray(Uint32 RGB_pixel);
Uint8 RGB_to_red(Uint32 RGB_pixel);
Uint8 RGB_to_green(Uint32 RGB_pixel);
Uint8 RGB_to_blue(Uint32 RGB_pixel);

// Convert red/green/blue values back into RGB pixel representation
Uint32 pack_RGB(Uint32 red_value, Uint32 green_value, Uint32 blue_value);

#endif
