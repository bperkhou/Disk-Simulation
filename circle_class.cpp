#include "circle_class.h"
#include <iostream>
using namespace std;

Circle::Circle(Uint32 pixel){
	c.first = 0;
	c.second = 0;
	r = 0;
	v.first = 0;
	v.second = 0;
	color = pixel;
}

Circle::Circle(std::pair <float, float> cin, float radius, Uint32 pixel){
	c = cin;
	r = radius;
	v.first = 0;
	v.second = 0;
	color = pixel;
}

Circle::Circle(std::pair <float, float> cin, float radius, std::pair <float, float> vin, Uint32 pixel){
	c = cin;
	r = radius;
	v = vin;
	color = pixel;
}




void Circle::set_c(std::pair <float, float> cin){c=cin;}
void Circle::set_v(std::pair <float, float> vin){v=vin;}
void Circle::set_r(float radius) {r=radius;}
void Circle::set_color(Uint32 pixel) {color = pixel;}

std::pair <float, float> Circle::get_c(){return c;}
std::pair <float, float> Circle::get_v(){return v;} 
float Circle::get_r(){return r;}

void Circle::render(SDL_Surface *surface){
	render_backend(surface, r, color);
}

bool Circle::resolve_wall_xcollisions(float w, float t){
	bool check_collision = false;
	if(c.first+v.first*t+r >= w){
		c.first = 2*w-2*r-c.first-v.first*t;
		v.first = -v.first;
		check_collision = true;
	}
	if(c.first+v.first*t-r < 0){
		c.first = 2*r-c.first-v.first*t;
		v.first = -v.first;
		check_collision = true;
	}
	return check_collision;
}

bool Circle::resolve_wall_ycollisions(float h, float t){
	bool check_collision = false;
	if(c.second+v.second*t+r >= h){
		c.second = 2*h-2*r-c.second-v.second*t;
		v.second = -v.second;
		check_collision = true;
	}
	if(c.second+v.second*t-r < 0){
		c.second = 2*r-c.second-v.second*t;
		v.second = -v.second;
		check_collision = true;
	}
	return check_collision;
}

void Circle::move(SDL_Surface *surface, float t){
	if(!resolve_wall_xcollisions(surface->w, t)){
		c.first += v.first*t;
	}
	if(!resolve_wall_ycollisions(surface->h, t)){
		c.second += v.second*t;
	}
}

void Circle::set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel){
    Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * sizeof(surface->pixels);
    *(Uint32 *)target_pixel = pixel;
}

void Circle::render_backend(SDL_Surface *surface, float r, Uint32 pixel){
	for(int x = (int)(c.first-r); x <= (int)(c.first+r); x++){
		for(int y = (int)(c.second -r); y <= (int)(c.second+r); y++){
			if((x >= 0) && (y >= 0) && (x < surface->w) && (y < surface->h) && (((float)x-c.first)*((float)x-c.first)+((float)y-c.second)*((float)y-c.second) <= r*r+.8*r))
				set_pixel(surface, x, y, pixel);
		}
	}
}