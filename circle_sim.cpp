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

int main( int argc, char* args[] ){
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
	
	if(!init(&window, &screenSurface)){
		printf("Failed to initialize");
		return 1;
	}

	
	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
	Circle circle(30,screenSurface->h / 2, 30, 420, 0,SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0xFF));
	circle.render(screenSurface);
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
		circle.move(screenSurface, dt);
		SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
		circle.render(screenSurface);
		SDL_UpdateWindowSurface(window);
		frame_end = SDL_GetTicks();
		if (frame_end - frame_start < 17)
			SDL_Delay(17 - frame_end + frame_start);
	}
	
	
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}

bool init(SDL_Window** window, SDL_Surface** surface){
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
			printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
			return false;
		}
	*window = SDL_CreateWindow( "Intellectually Delayed Paint", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if( *window == NULL ){
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return false;
	}
	*surface = SDL_GetWindowSurface( *window );
	return true;
}
