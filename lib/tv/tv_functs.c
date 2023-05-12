#define _GNU_SOURCE

#include <SDL2/SDL.h>
#include "tv.h"
#include <stdio.h>



uint8_t pos_bounds_check( int x, int y ){ return (x>=0 && x<=WIDTH) && (y>=0 && y<=HEIGHT); }
int pos_bounds_wrapX( int *x ){ *x %= WIDTH; }
int pos_bounds_wrapY( int *y ){ *y %= HEIGHT; }
