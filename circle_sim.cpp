#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "circle_class.h"
#include "all_disks.h"
#include <string>
#include <sstream>
#include <utility>
#include <math.h>


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init(SDL_Window** window, SDL_Surface** surface);
/*
void circle_collision(Circle *a, Circle *b, float t);
float col_time(Circle *a, Circle *b);
float dot(std::pair <float, float> a, std::pair <float, float> b);
std::pair <float, float> add(std::pair <float, float> a, std::pair <float, float> b);
std::pair <float, float> sub(std::pair <float, float> a, std::pair <float, float> b);
std::pair <float, float> mult(float a, std::pair <float, float> b);
*/


int main( int argc, char* args[] ){
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;

	if(!init(&window, &screenSurface)){
		printf("Failed to initialize");
		return 1;
	}


	//std::pair <float, float> c1(30,screenSurface->h/2);
	//std::pair <float, float> v1(100, 0);
	//std::pair <float, float> c2(screenSurface->w-30,screenSurface->h/2);
	//std::pair <float, float> v2(0, 0);
	
	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
	Disks system(2, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0xFF));
	system.render_all(screenSurface);

	//Circle circle1(c1, 30, v1, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0xFF));
	//Circle circle2(c2, 30, v2, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0xFF));
	//circle1.render(screenSurface);
	//circle2.render(screenSurface);
	SDL_UpdateWindowSurface(window);


	bool quit = false;
	SDL_Event e;

	float dt;
	int cur = SDL_GetTicks();
	int prev;
	int frame_start;
	int frame_end;

	//main loop
	while(!quit){
		frame_start = SDL_GetTicks();
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT)
				quit = true;
		}	
		prev = cur;
		cur = SDL_GetTicks();
		dt = (cur - prev)/(1000.f);
		
		system.update(screenSurface, dt);
		SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
		system.render_all(screenSurface);
		//circle_collision(&circle1, &circle2, dt);

		//circle1.move(screenSurface, dt);
		//circle2.move(screenSurface, dt);
		//circle1.render(screenSurface);
		//circle2.render(screenSurface);
		SDL_UpdateWindowSurface(window);
		frame_end = SDL_GetTicks();
		
		
		if (frame_end - frame_start < 17)
			SDL_Delay(17 - frame_end + frame_start);
	}
	
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}

/*
void circle_collision(Circle *a, Circle *b, float t){
	std::pair <float, float> dc = sub(a->get_c(), b->get_c());
	std::pair <float, float> dv = sub(a->get_v(), b->get_v());

	float ar = a->get_r();
	float br = b->get_r();

	float distsq = dot(dc, dc);
	float K = dot(dc, dv);

	//checks collisions and if distance between circles is decreasing
	if((K < 0) && ((ar+br)*(ar+br) > distsq)){
		K = K / distsq;
		float ma = ar*ar;
		float mb = br*br;
		float mbovertot = mb / (ma + mb);
		float maovertot = ma / (ma + mb);
		a->set_v(sub(a->get_v(),mult(2*mbovertot*K,dc)));
		b->set_v(add(b->get_v(),mult(2*maovertot*K,dc)));
	}
}


//returns -1 if circles don't collide or are tangential, otherwise time of first collision
float col_time(Circle* a, Circle* b){
	std::pair <float, float> dc = sub(a->get_c(), b->get_c());
	std::pair <float, float> dv = sub(a->get_v(), b->get_v());
	float ar = a->get_r();
	float br = b->get_r();
	
	float cdotv = dot(dc, dv);
	float cdistsq = dot(dc, dc);
	float vdistsq = dot(dv, dv);
	float rsumsq = (ar+br)*(ar+br);
	
	float insqrt = cdotv*cdotv-vdistsq*(cdistsq-rsumsq);
	if(insqrt <= 0 || cdistsq < rsumsq || vdistsq == 0) return -1;
	
	return (-sqrt(insqrt)-cdotv)/vdistsq;
}


float dot(std::pair <float, float> a, std::pair <float, float> b){
	return a.first*b.first+a.second*b.second;
}
std::pair <float, float> add(std::pair <float, float> a, std::pair <float, float> b){
	std::pair <float, float> result(a.first+b.first, a.second+b.second);
	return result;
}
std::pair <float, float> sub(std::pair <float, float> a, std::pair <float, float> b){
	std::pair <float, float> result(a.first-b.first, a.second-b.second);
	return result;
}

std::pair <float, float> mult(float a, std::pair <float, float> b){
	std::pair <float, float> result(a*b.first, a*b.second);
	return result;
}
*/

bool init(SDL_Window** window, SDL_Surface** surface){
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
			printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
			return false;
		}
	*window = SDL_CreateWindow( "Intellectually Delayed Disk Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if( *window == NULL ){
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return false;
	}
	*surface = SDL_GetWindowSurface( *window );
	return true;
}
