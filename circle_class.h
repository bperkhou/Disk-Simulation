#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <SDL.h>


class Circle {
	public:
		Circle(Uint32 pixel);
		Circle(float x, float y, float radius, Uint32 pixel);
		Circle(float x, float y, float radius, float xvel, float yvel, Uint32 pixel);
		void set_xc(float x);
		void set_yc(float y);
		void set_c(float x, float y);
		void set_xv(float x);
		void set_yv(float y);
		void set_v(float x, float y);
		void set_r(float radius);
		void set_color(Uint32 pixel);
		
		float get_xc();
		float get_yc();
		float get_xv();
		float get_yv();
		float get_r();
		
		void render(SDL_Surface *surface);
		void move(SDL_Surface *surface, float t);

	private:
		void render_backend(SDL_Surface *surface, float xc, float yc, float r, Uint32 pixel);
		void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
		bool resolve_collisions(SDL_Surface *surface, float t);
		bool resolve_wall_collisions(float w, float h, float t);
		Uint32 color;
		float xv;
		float yv;
		float xc;
		float yc;
		float r;
};
