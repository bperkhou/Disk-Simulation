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
void circle_collision(Circle *a, Circle *b);

int main( int argc, char* args[] ){
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
	
	if(!init(&window, &screenSurface)){
		printf("Failed to initialize");
		return 1;
	}

	
	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
	Circle circle1(30,screenSurface->h / 2, 30, 200, 300,SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0xFF));
	Circle circle2(screenSurface->w-65,screenSurface->h / 2, 65, 420, -10,SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0xFF));
	circle1.render(screenSurface);
	circle2.render(screenSurface);
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
		circle_collision(&circle1, &circle2);
		prev = cur;
		cur = SDL_GetTicks();
		dt = (cur - prev)/(1000.f);
		
		circle1.move(screenSurface, dt);
		circle2.move(screenSurface, dt);
		SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
		circle1.render(screenSurface);
		circle2.render(screenSurface);
		SDL_UpdateWindowSurface(window);
		frame_end = SDL_GetTicks();
		
		
		if (frame_end - frame_start < 17)
			SDL_Delay(17 - frame_end + frame_start);
	}
	
	
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}

void circle_collision(Circle *a, Circle *b){
	float axc = a->get_xc();
	float ayc = a->get_yc();
	float bxc = b->get_xc();
	float byc = b->get_yc();
	float dx = axc - bxc;
	float dy = ayc - byc;
	float ar = a->get_r();
	float br = b->get_r();
	float axv = a->get_xv();
	float ayv = a->get_yv();
	float bxv = b->get_xv();
	float byv = b->get_yv();
	float distsq = dx*dx+dy*dy;
	float aorient = -(axv*dx+ayv*dy);
	float borient = bxv*dx+byv*dy;
	if(!((aorient < 0) && (borient < 0)) && ((ar+br)*(ar+br) > distsq)){

		float K = -(aorient + borient);
		K = K / distsq;
		float ma = ar*ar;
		float mb = br*br;
		a->set_xv(axv-(2 * (mb / (ma + mb)) * K * dx));
		a->set_yv(ayv-(2 * (mb / (ma + mb)) * K * dy));
		b->set_xv(bxv+(2 * (ma / (ma + mb)) * K * dx));
		b->set_yv(byv+(2 * (ma / (ma + mb)) * K * dy));
	}
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
