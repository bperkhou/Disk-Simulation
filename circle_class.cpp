#include "circle_class.h"
#include <iostream>
using namespace std;

Circle::Circle(Uint32 pixel){
	xc = 0;
	yc = 0;
	r = 0;
	xv = 0;
	yv = 0;
	color = pixel;
}

Circle::Circle(float x, float y, float radius, Uint32 pixel){
	xc = x;
	yc = y;
	r = radius;
	xv = 0;
	yv = 0;
	color = pixel;
}

Circle::Circle(float x, float y, float radius, float xvel, float yvel, Uint32 pixel){
	xc = x;
	yc = y;
	r = radius;
	xv = xvel;
	yv = yvel;
	color = pixel;
}



void Circle::set_xc(float x) {xc=x;}
void Circle::set_yc(float y) {yc=y;}
void Circle::set_c(float x, float y){xc=x; yc=y;}
void Circle::set_xv(float x) {xv=x;}
void Circle::set_yv(float y) {yv=y;}
void Circle::set_v(float x, float y){xv=x; yv=y;}
void Circle::set_r(float radius) {r=radius;}
void Circle::set_color(Uint32 pixel) {color = pixel;}

float Circle::get_xc(){return xc;}
float Circle::get_yc(){return yc;} 
float Circle::get_xv(){return xv;}
float Circle::get_yv(){return yv;}
float Circle::get_r(){return r;}

void Circle::render(SDL_Surface *surface){
	render_backend(surface, xc, yc, r, color);
}

bool Circle::resolve_collisions(SDL_Surface *surface, float t){
	return resolve_wall_collisions((float) surface->w, (float) surface->h, t);
}

bool Circle::resolve_wall_collisions(float w, float h, float t){
	bool check_collision = false;
	if(xc+xv*t+r >= w){
		xc = 2*w-2*r-xc-xv*t;
		xv = -xv;
		check_collision = true;
	}
	if(xc+xv*t-r < 0){
		xc = 2*r-xc-xv*t;
		xv = -xv;
		check_collision = true;
	}
	if(yc+yv*t+r >= h){
		yc = 2*h-2*r-yc-yv*t;
		yv = -yv;
		check_collision = true;
	}
	if(yc+yv*t-r < 0){
		yc = 2*r-yc-yv*t;
		yv = -yv;
		check_collision = true;
	}
	return check_collision;
}

void Circle::move(SDL_Surface *surface, float t){
	if(!resolve_collisions(surface, t)){
		xc += xv*t;
		yc += yv*t;
	}
}

void Circle::set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel){
    Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * sizeof(surface->pixels);
    *(Uint32 *)target_pixel = pixel;
}

void Circle::render_backend(SDL_Surface *surface, float xc, float yc, float r, Uint32 pixel){
	for(int x = (int)(xc-r); x <= (int)(xc+r); x++){
		for(int y = (int)(yc -r); y <= (int)(yc+r); y++){
			if((x >= 0) && (y >= 0) && (x < surface->w) && (y < surface->h) && (((float)x-xc)*((float)x-xc)+((float)y-yc)*((float)y-yc) <= r*r+.8*r))
				set_pixel(surface, x, y, pixel);
		}
	}
}