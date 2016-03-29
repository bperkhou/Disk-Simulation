#include "disk_class.h"
#include <iostream>

Disk::Disk(){}

Disk::Disk(Uint32 pixel) :
	c(0,0), r(0), v(0,0), color(pixel) {}


Disk::Disk(vec c, double r, Uint32 pixel) :
	c(c), r(r), v(0,0), color(pixel) {}


Disk::Disk(vec c, double r, vec v, Uint32 pixel) :
 c(c), r(r), v(v), color(pixel) {}

void Disk::set_c(vec cin){c=cin;}
void Disk::set_v(vec vin){v=vin;}
void Disk::set_r(double radius) {r=radius;}
void Disk::set_color(Uint32 pixel) {color = pixel;}

vec Disk::get_c(){return c;}
vec Disk::get_v(){return v;} 
double Disk::get_r(){return r;}

void Disk::render(SDL_Surface *surface){
	render_backend(surface, r, color);
}

void Disk::move(double t){
	c.first += v.first*t;
	c.second += v.second*t;
}

void Disk::set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel){
    Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * sizeof(surface->pixels);
    *(Uint32 *)target_pixel = pixel;
}

void Disk::render_backend(SDL_Surface *surface, double r, Uint32 pixel){
	for(int x = (int)(c.first-r); x <= (int)(c.first+r); x++){
		for(int y = (int)(c.second -r); y <= (int)(c.second+r); y++){
			if((x >= 0) && (y >= 0) && (x < surface->w) && (y < surface->h) && (((double)x-c.first)*((double)x-c.first)+((double)y-c.second)*((double)y-c.second) <= r*r+.8*r))
				set_pixel(surface, x, y, pixel);
		}
	}
}

void Disk::print_disk(){
	std::cout << "                    _________________________________________" << std::endl <<
				 "The disk stats are: |   r   |  cx   |  cy   |  vx   |  vy   |" << std::endl <<
				 "                    -----------------------------------------" << std::endl <<
				 "                    |" << r << "|" << c.first << "|" << c.second << "|" <<
				 v.first << "|" << v.second << "|" << std::endl <<
				 "                    -----------------------------------------" << std::endl;
}