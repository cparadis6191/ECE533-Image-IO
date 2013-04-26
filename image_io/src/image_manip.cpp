#include "image_manip.h"

#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>


using namespace std;

int main(int argc, char** argv) {
	// Initialize command line flags
	// Threshold flags
	int t_flag = 0;
	int t_value = 0;

	// Dilation flags
	int d_flag = 0;
	int d_value = 0;

	// Erosion flags
	int r_flag = 0;
	int r_value = 0;

	// Sobel gradient flag
	int g_flag = 0;

	// Laplace flag
	int l_flag = 0;

	// Perimiter flag
	int p_flag = 0;

	// Area flag
	int a_flag = 0;

	// Moment flag
	int m_flag = 0;

	// Invariant flag
	int v_flag = 0;

	// Eigen flag
	int e_flag = 0;

	// Invert flag
	int i_flag = 0;

	// Smooth method
	int s_med_flag = 0;
	int s_mean_flag = 0;
	string s_args;

	// Histogram equalization flag
	int h_flag = 0;

	// Color mask flags
	int c_flag = 0;
	int c_r_flag = 0;
	int c_g_flag = 0;
	int c_b_flag = 0;
	int c_mask = 0;
	string c_args;

	char* output_file = NULL;
	char* input_file = NULL;
	char c;


	// If no command-line arguments are passed
	if (argc < 2) {
		printf("Usage: io_manip -f [INPUT]... -o [OUTPUT]... [OPTION]...\n");

		return 1;
	}


	// Parse through all the arguments
	while ((c = getopt(argc, argv, "f:o:t:d:r:glpamveishc:")) != -1) {
		switch (c) {
			// Input file
			case 'f':
				input_file = optarg;
				
				break;


			// Output file
			case 'o':
				output_file = optarg;
				
				break;


			// Threshold the image
			case 't':
				t_flag = 1;
				t_value = atoi(optarg);
				
				break;


			// Dilate the image
			case 'd':
				d_flag = 1;
				d_value = atoi(optarg);
				
				break;


			// Erode the image
			case 'r':
				r_flag = 1;
				r_value = atoi(optarg);
				
				break;


			// Apply a Sobel gradient to the image
			case 'g':
				g_flag = 1;
				
				break;


			// Apply a Sobel gradient to the image
			case 'l':
				l_flag = 1;
				
				break;


			// Compute the perimiter of the object
			case 'p':
				p_flag = 1;
				
				break;


			// Compute the area of the object
			case 'a':
				a_flag = 1;
				
				break;


			// Compute the moments of the object
			case 'm':
				m_flag = 1;
				
				break;

			// Compute the moment invariants of the object
			case 'v':
				v_flag = 1;
				
				break;


			// Compute the Eigen values and vectors
			case 'e':
				e_flag = 1;
				
				break;


			// Invert the specified channels of the image
			case 'i':
				i_flag = 1;
				
				break;


			// Smooth the image with mean algorithm
			case 's':
				s_args = optarg;

				// Check the arguments for smoothing method
				if ((s_args.find("m") != s_args.npos)) (s_mean_flag = 1);
				if ((s_args.find("d") != s_args.npos)) (s_med_flag = 1);

				break;


			// Apply histogram equalization algorithm to the image
			case 'h':
				h_flag = 1;

				break;


			// Color mask
			case 'c':
				c_flag = 1;
				c_args = optarg;

				// Check the arguments for RGB flags and set the color mask
				if ((c_args.find("r") != c_args.npos)) (c_r_flag = 1);
				if ((c_args.find("g") != c_args.npos)) (c_g_flag = 1);
				if ((c_args.find("b") != c_args.npos)) (c_b_flag = 1);
				
				break;


			// Error checking
			case '?':
			default:
				if ((optopt == 'f') || (optopt == 'o')) {
					printf("Option -%c requires a file as an argument.\n", optopt);
				} else if ((optopt == 'c')) {
					printf("Option -%c requires an argument.\nPass the flags 'r', 'g' or 'b' to mask off those color channels.\n", optopt);
				} else if ((optopt == 's')) {
					printf("Option -%c requires an argument.\nPass the flags 'd' or 'm' to use a specific smoothing method.\n", optopt);
				} else if (isprint(optopt)) {
					printf("Unknown option '-%c'.\n", optopt);
				} else {
					printf("Unknown option character '\\x%x'.\n", optopt);
				}


				return 1;
		}
	}

	// Check for input and output files
	if (!input_file) {
		cout << "Please specify an input file!\n";

		return 1;
	}

	if (!output_file) {
		cout << "Please specify an output file!\n";

		return 1;
	}


	// Initialize the SDL libraries
	SDL_Init(SDL_INIT_EVERYTHING);


	// Open the image
	image_io* image = new image_io(input_file);


	// Do the the operations specified by the command line switches
	// Operations in roughly ascending order of destructiveness
	// Compose the mask and mask off specified colors
	c_mask = (c_r_flag*M_RED | c_g_flag*M_GREEN | c_b_flag*M_BLUE);
	if (c_flag) color_mask(image, c_mask);
	if (i_flag) invert(image);

	if (s_mean_flag) smooth_mean(image);
	if (s_med_flag) smooth_median(image);

	if (h_flag) hist_eq(image);

	if (t_flag) threshold(image, t_value);
	if (d_flag) dilation(image, d_value);
	if (r_flag) erosion(image, r_value);
	if (p_flag) {
		cout << "Perimiter is: " << perimiter(image) << endl;
	}
	if (a_flag) {
		cout << "Area is: " << area(image) << endl;
	}
	if (m_flag || v_flag || e_flag) {
		double** moment_results = moment(image);
		double* centroid_results = centroid(moment_results);
		double** central_moment_results = central_moments(moment_results, centroid_results);
		double* invariant_results;
		double** eigen_results;
		
		if (m_flag) {
			cout << "M00 is: " << moment_results[0][0] << endl;
			cout << "M01 is: " << moment_results[0][1] << endl;
			cout << "M02 is: " << moment_results[0][2] << endl;
			cout << "M03 is: " << moment_results[0][3] << endl;
			cout << "M10 is: " << moment_results[1][0] << endl;
			cout << "M20 is: " << moment_results[2][0] << endl;
			cout << "M30 is: " << moment_results[3][0] << endl;
			cout << "M11 is: " << moment_results[1][1] << endl;
			cout << "M12 is: " << moment_results[1][2] << endl;
			cout << "M21 is: " << moment_results[2][1] << endl;

			cout << "Centroid is: (" << centroid_results[0] << ", ";
			cout << centroid_results[1] << ")" << endl;

			cout << "U00 is: " << central_moment_results[0][0] << endl;
			cout << "U02 is: " << central_moment_results[0][2] << endl;
			cout << "U03 is: " << central_moment_results[0][3] << endl;
			cout << "U20 is: " << central_moment_results[2][0] << endl;
			cout << "U30 is: " << central_moment_results[3][0] << endl;
			cout << "U11 is: " << central_moment_results[1][1] << endl;
			cout << "U12 is: " << central_moment_results[1][2] << endl;
			cout << "U21 is: " << central_moment_results[2][1] << endl;
		}

		if (v_flag) {
			invariant_results = invariants(central_moment_results);

			cout << "I0 is: " << invariant_results[0] << endl;
			cout << "I1 is: " << invariant_results[1] << endl;
			cout << "I2 is: " << invariant_results[2] << endl;
			cout << "I3 is: " << invariant_results[3] << endl;
			cout << "I4 is: " << invariant_results[4] << endl;
			cout << "I5 is: " << invariant_results[5] << endl;
			cout << "I6 is: " << invariant_results[6] << endl;

			delete[] invariant_results;
		}
		
		if (e_flag) {
			eigen_results = eigen(moment_results, centroid_results);

			cout << "L1 is: " << eigen_results[0][0] << endl;
			cout << "L2 is: " << eigen_results[1][0] << endl;

			cout << "V1 is: (" << eigen_results[0][1] << ", " << eigen_results[0][2] << ")" << endl;
			cout << "V2 is: (" << eigen_results[1][1] << ", " << eigen_results[1][2] << ")" << endl;

			for (int i = 0; i < 2; i++) {
				delete[] eigen_results[i];
			}
			delete[] eigen_results;
		}

		// Free memory C-style!
		for (int i = 0; i < 4; i++) {
			delete[] moment_results[i];
			delete[] central_moment_results[i];
		}


		delete[] moment_results;
		delete[] central_moment_results;
		delete[] centroid_results;
	}

	// Edge Detection
	if (g_flag) sobel_gradient(image);
	if (l_flag) laplacian(image);


	// Write to a new image file
	image->write(output_file);

	// Free memory from the images
	delete image;

	// Cleans up and closes the SDL libraries
	SDL_Quit();


	return 0;
}
