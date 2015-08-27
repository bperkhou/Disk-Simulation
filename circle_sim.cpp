#include <SDL.h>
#include <stdio.h>
#include<iostream>
#include <stdlib.h>
#include "circle_class.h"



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
	SDL_UpdateWindowSurface( window );

	bool quit = false;
	SDL_Event e;
	
	Circle placeholder(0,0,30);
	
	while(!quit){
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_MOUSEBUTTONDOWN){
				placeholder.set_xc(e.button.x);
				placeholder.set_yc(e.button.y);
				placeholder.fill_disk(screenSurface, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0xFF));
				SDL_UpdateWindowSurface(window);
			}
			if(e.type == SDL_QUIT)
				quit = true;
		}

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
