ECE533-Image-Manip
========================

An image manipulation program written in C++ using the SDL and SDL_Image libraries. The program implements some simple functions (e.g. color masking and inversion) as well as some more complex algorithms such as a Sobel gradient filter or histogram equalization.

### Compilation

Have the SDL and SDL_Image development libraries installed. Type make while in the src directory.

```bash
cd src
make
```

### Usage

Basic usage is as follows:

```bash
./image_manip -f [input image] -o [output image] [flags]
```

### Examples

Original image taken from [Wikipedia.org](http://en.wikipedia.org/wiki/South_China_tiger#mediaviewer/File:2012_Suedchinesischer_Tiger.JPG)

![Original image](https://github.com/cparadis6191/ECE533-Image-Manip/blob/master/examples/tiger.jpg)

Input can be in a variety of formats. Output files are bitmaps but have been converted to jpgs for size reasons.

```bash
./image_manip -f tiger.jpg -o histogram.jpg -h
```
![Histogram Equalization](https://github.com/cparadis6191/ECE533-Image-Manip/blob/master/examples/histogram.jpg)

```bash
./image_manip -f tiger.jpg -o laplace.jpg -l
```
![Laplacian transform](https://github.com/cparadis6191/ECE533-Image-Manip/blob/master/examples/laplace.jpg)

```bash
./image_manip -f tiger.jpg -o sobel_mean.jpg -sm
```
![Sobel mean](https://github.com/cparadis6191/ECE533-Image-Manip/blob/master/examples/sobel_mean.jpg)

```bash
./image_manip -f tiger.jpg -o sobel_median.jpg -sd
```
![Sobel median](https://github.com/cparadis6191/ECE533-Image-Manip/blob/master/examples/sobel_median.jpg)

```bash
./image_manip -f tiger.jpg -o red.jpg -cgb
```
![Color masking](https://github.com/cparadis6191/ECE533-Image-Manip/blob/master/examples/red.jpg)

```bash
./image_manip -f tiger.jpg -o invert.jpg -i
```
![Color inversion](https://github.com/cparadis6191/ECE533-Image-Manip/blob/master/examples/invert.jpg)

```bash
./image_manip -f tiger.jpg -o threshhold_128.jpg -t128
```
![Threshholding](https://github.com/cparadis6191/ECE533-Image-Manip/blob/master/examples/threshhold_128.jpg)

```bash
./image_manip -f tiger.jpg -o threshhold_128_eroded.jpg -t128 -r1
```
![Erosion](https://github.com/cparadis6191/ECE533-Image-Manip/blob/master/examples/threshhold_128_eroded.jpg)
