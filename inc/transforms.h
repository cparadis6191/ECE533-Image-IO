#pragma once

#include "image_io.h"

#include <array>


// Color mask flags
#define M_RED (1 << 0)
#define M_GREEN (1 << 1)
#define M_BLUE (1 << 2)

// Forward declaration
class image_io;

class locker {
	public:
		locker(image_io& image_src);
		~locker();

	private:
		image_io& m_image;
};

// Choose a color to mask off
// gray = (0.299*r + 0.587*g + 0.114*b);
void color_mask(image_io& image_src, int mask);
// Invert color intensity
void invert(image_io& image_src);

// Apply a smoothing effect
// Utilizes a 3x3 neighborhood averaging algorithm
void smooth_mean(image_io& image_src);
// Utilizes a 3x3 neighborhood median algorithm
void smooth_median(image_io& image_src);

// Adjust constrast with histogram equalization algorithm
void hist_eq(image_io& image_src);

// Convert an image into a binary (black/white) image splitting at the threshold. All pixels equal to or greater than the threshold will be turned white, all pixels below will be black
void threshold(image_io& image_src, Uint32 threshold);

// Edge detection using the Sobel Gradient
void sobel_gradient(image_io& image_src);
// Edge detection using Laplacian Transformt
void laplacian(image_io& image_src);

// Degrade the image by n pixels
void erosion(image_io& image_src, int erode_n);
// Enlarge the iamge by n pixels
void dilation(image_io& image_src, int dilate_n);

// Compute the perimeter
int perimiter(image_io& image_src);
// Compute the area
int area(image_io& image_src);

// Compute the moment
// Returns a 4x4 matrix
std::array<std::array<double, 4>, 4> moment(image_io& image_src);

// Compute the centroid from the moment
// Returns an array of two (x, y)
std::array<double, 2> centroid(const std::array<std::array<double, 4>, 4>& M);

// Compute the central_moments
// Returns an array of the central moments
std::array<std::array<double, 4>, 4> central_moments(const std::array<std::array<double, 4>, 4>& M, const std::array<double, 2>& C);

// Calculate the 7 moment invariants
// u is a 4x4 matrix containing central moment values
std::array<double, 7> invariants(const std::array<std::array<double, 4>, 4>& u);

// Calculate the eigenvalues and eigenvectors of the covariance matrix
// Returns 2x3 matrix
std::array<std::array<double, 2>, 3> eigen(const std::array<std::array<double, 4>, 4>& M, const std::array<double, 2>& C);

// Convert an RGB pixel representation to a grayscale value
Uint8 RGB_to_gray(Uint32 RGB_pixel);
Uint8 RGB_to_red(Uint32 RGB_pixel);
Uint8 RGB_to_green(Uint32 RGB_pixel);
Uint8 RGB_to_blue(Uint32 RGB_pixel);

// Convert red/green/blue values back into RGB pixel representation
Uint32 pack_RGB(Uint8 red_value, Uint8 green_value, Uint8 blue_value);
