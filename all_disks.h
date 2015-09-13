#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <SDL.h>
#include <utility>
#include "circle_class.h"
#include <math.h>


//ASSUMES screen width 640 and height 480
class Disks {
	public:
		Disks(int num, Uint32 pixel);
		~Disks();
		void render_all(SDL_Surface *surface);
		void update(SDL_Surface *surface, float t);
		static const float rmin = 5;
		static const float rmax = 100;
		static const float vmax = 100;
		static const int SCREEN_WIDTH = 640;
		static const int SCREEN_HEIGHT = 480;

	private:
		int n;
		Circle *circles;
		//returns time remaining in current frame after resolving collisions
		float resolve_collisions(SDL_Surface *surface, float t);
		void move(SDL_Surface *surface, float t);
		float col_time(Circle *a, Circle *b);
		float dot(std::pair <float, float> a, std::pair <float, float> b);
		std::pair <float, float> add(std::pair <float, float> a, std::pair <float, float> b);
		std::pair <float, float> sub(std::pair <float, float> a, std::pair <float, float> b);
		std::pair <float, float> mult(float a, std::pair <float, float> b);
		void col_update(Circle *a, Circle *b);
		
};
