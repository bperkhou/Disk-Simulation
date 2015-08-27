#include "circle_class.h"
#include <iostream>
using namespace std;

Circle::Circle(int x, int y, int radius){
	xc = x;
	yc = y;
	r = radius;
}

void Circle::set_xc(int x) {xc=x;}
void Circle::set_yc(int y) {yc=y;}
void Circle::set_r(int radius) {r=radius;}

void Circle::fill_disk(SDL_Surface *surface, Uint32 pixel){
	fill_disk_backend(surface, xc, yc, r, pixel);
}

void Circle::set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel){
    Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * sizeof(surface->pixels);
    *(Uint32 *)target_pixel = pixel;
}

void Circle::fill_disk_backend(SDL_Surface *surface, int xc, int yc, int r, Uint32 pixel){
	for(int x = xc-r; x <= xc+r; x++){
		for(int y = yc -r; y<= yc+r; y++){
			if( (x >= 0) && (y >= 0) && (x < surface->w) && (y < surface->h) && (5*(x-xc)*(x-xc)+5*(y-yc)*(y-yc) <= 5*r*r+4*r))
				set_pixel(surface, x, y, pixel);
		}
	}
}



//MESSAGE FOR GITHUB DID IT WORK?
