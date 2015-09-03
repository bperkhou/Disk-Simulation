#include <SDL.h>
#include <stdio.h>
#include<iostream>
#include <stdlib.h>
#include "circle_class.h"
#include <string>
#include <sstream>



//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init(SDL_Window** window, SDL_Surface** surface);
void circle_collision(Circle *a, Circle *b, float t);
float col_time(Circle a, Circle b);


int main( int argc, char* args[] ){
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;

	if(!init(&window, &screenSurface)){
		printf("Failed to initialize");
		return 1;
	}


	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
	Circle circle1(215,screenSurface->h/2, 45, 1000, 0,SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0xFF));
	Circle circle2(300,screenSurface->h/2, 45, 0, 0,SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0xFF));
	//Circle circle3(0, screenSurface->h/2, 45, 1000, 0,SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0xFF));
	circle1.render(screenSurface);
	circle2.render(screenSurface);
	//circle3.render(screenSurface);
	SDL_UpdateWindowSurface( window );

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
		circle_collision(&circle1, &circle2, dt);
		//circle_collision(&circle2, &circle3, dt);
		//circle_collision(&circle3, &circle1, dt);
		
		circle1.move(screenSurface, dt);
		circle2.move(screenSurface, dt);
		//circle3.move(screenSurface, dt);
		SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
		circle1.render(screenSurface);
		circle2.render(screenSurface);
		//circle3.render(screenSurface);
		SDL_UpdateWindowSurface(window);
		frame_end = SDL_GetTicks();
		
		
		if (frame_end - frame_start < 17)
			SDL_Delay(17 - frame_end + frame_start);
	}
	
	
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}

void circle_collision(Circle *a, Circle *b, float t){
	float axc = a->get_xc();
	float ayc = a->get_yc();
	float bxc = b->get_xc();
	float byc = b->get_yc();
	float dx = axc - bxc;
	float dy = ayc - byc;

	float axv = a->get_xv();
	float ayv = a->get_yv();
	float bxv = b->get_xv();
	float byv = b->get_yv();
	float dxv = axv - bxv;
	float dyv = ayv - byv;

	float ar = a->get_r();
	float br = b->get_r();

	float distsq = dx*dx+dy*dy;
	float K = dx*dxv+dy*dyv;

	//checks collisions and if distance between circles is decreasing
	if((K < 0) && ((ar+br)*(ar+br) > distsq)){
		K = K / distsq;
		float ma = ar*ar;
		float mb = br*br;
		float mbovertot = mb / (ma + mb);
		float maovertot = ma / (ma + mb);
		a->set_xv(axv-(2 * mbovertot * K * dx));
		a->set_yv(ayv-(2 * mbovertot * K * dy));
		b->set_xv(bxv+(2 * maovertot * K * dx));
		b->set_yv(byv+(2 * maovertot * K * dy));
	}
}

float col_time(Circle* a, Circle* b){
	float ar = a->get_r();
	float br = b->get_r();
	
	float axc = a->get_xc();
	float bxc = b->get_xc();
	float ayc = a->get_yc();
	float byc = b->get_yc();
	
	float axv = a->get_xv();
	float bxv = b->get_xv();
	float ayv = a->get_yv();
	float byv = b->get_yv();
	
	

	
}

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
