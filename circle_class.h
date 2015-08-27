#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <SDL.h>


class Circle {
	public:
		Circle(int x, int y, int radius);
		void set_xc(int x);
		void set_yc(int y);
		void set_r(int radius);
		void fill_disk(SDL_Surface *surface, Uint32 pixel);
		

	private:
		void fill_disk_backend(SDL_Surface *surface, int xc, int yc, int r, Uint32 pixel);
		void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
		int xc;
		int yc;
		int r;
};
