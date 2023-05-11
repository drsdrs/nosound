#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "../SDL_inprint/SDL2_inprint.h"
#include "tv.h"
#include "../looper/looper.h"

SDL_Window* window;
SDL_Surface* tv;
SDL_Surface* pixels;
SDL_Renderer* renderer;
SDL_Joystick* gamepad;

uint16_t RES_W = 640;
uint16_t RES_H = 480;
uint16_t SOURCE_RES_W = 640;
uint16_t SOURCE_RES_H = 480;

uint8_t CHAR_HEIGHT = 8;
uint8_t CHAR_HEIGHT2 = 4;

uint16_t WIDTH, WIDTH2;
uint16_t HEIGHT, HEIGHT2;
uint8_t SCALE;

uint8_t ROWS;
uint8_t LINES;

const int JOYSTICK_DEAD_ZONE = 8000;

void tv_exit(){
  static uint8_t exit_flag = false;
  if( exit_flag==true ) return;
  exit_flag = true;
  printf("TV_Exit\n");
	SDL_JoystickClose( gamepad );
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	SDL_Quit();
	kill_inline_font();
  //loop_exit();
}

#include "tv_functs.c"

/*  scale max:0/1:(640,480) 1/2:(320,240) 2/4:(160,120) 3/8:(80,60) 4/16:(40,30) 5/32:(20,15) */


uint8_t tv_scale_set( int8_t scale){
  if( scale>5 || scale<0){ printf("Wrong scale: %i\n", scale); return 0; }
  //printf("setScale: %i\n", SCALE);
  SCALE = 1<<scale;
  SDL_RenderSetScale(renderer, SCALE, SCALE);


  WIDTH = SOURCE_RES_W / SCALE;
  HEIGHT = SOURCE_RES_H / SCALE;
  HEIGHT2 = HEIGHT / 2;
  WIDTH2 = WIDTH / 2;
  ROWS = WIDTH/8;
  LINES = HEIGHT/8;
  return 1;
}



void initWindow( uint8_t scale ){
  RES_W *= 1;
  RES_H *= 1;

  window = SDL_CreateWindow(
    "SDL pixels", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    RES_W, RES_H,
    SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
  );
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	tv = SDL_GetWindowSurface(window);
  tv_scale_set( scale );
}

void tv_setup(){
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0)    {
		printf("SDL could not be initialized!\n"
		"SDL_Error: %s\n", SDL_GetError());
	}

	initWindow( 1 );

	gamepad = SDL_JoystickOpen( 0 );

  inrenderer(renderer);
  prepare_inline_font();

}
