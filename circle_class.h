#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <SDL.h>
#include <utility>

#ifndef CIRCLE_CLASS
#define CIRCLE_CLASS

class Circle {
	public:
		Circle();
		Circle(Uint32 pixel);
		Circle(std::pair <float, float> cin, float radius, Uint32 pixel);
		Circle(std::pair <float, float> cin, float radius, std::pair <float, float> vin, Uint32 pixel);
		void set_c(std::pair <float, float> cin);
		void set_v(std::pair <float, float> vin);
		void set_r(float radius);
		void set_color(Uint32 pixel);
		
		std::pair <float, float> get_c();
		std::pair <float, float> get_v();
		float get_r();
		
		void render(SDL_Surface *surface);
		void move(SDL_Surface *surface, float t);

	private:
		void render_backend(SDL_Surface *surface, float r, Uint32 pixel);
		void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
		bool resolve_wall_xcollisions(float w, float t);
		bool resolve_wall_ycollisions(float h, float t);
		Uint32 color;
		std::pair <float, float> c;
		std::pair <float, float> v;
		float r;
};

#endif /* CIRCLE_CLASS */