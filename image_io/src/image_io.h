#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


Uint32 get_pixel32(SDL_Surface* surface, int x, int y);
void put_pixel32(SDL_Surface* surface, int x, int y, Uint32 pixel);

// gray = (0.299*r + 0.587*g + 0.114*b);
void RGB_to_Grayscale(SDL_Surface* surface);
