#pragma once

#include <SDL2/SDL.h>

#include "tv_ASCII.h"
#include "tv_COLORS.h"
#include "tv_KEYS.h"

extern SDL_Renderer* renderer;

extern uint16_t WIDTH, WIDTH2;
extern uint16_t HEIGHT, HEIGHT2;
extern uint8_t  CHAR_HEIGHT, CHAR_HEIGHT2;
extern uint8_t  SCALE;

extern uint8_t ROWS;
extern uint8_t LINES;

extern uint16_t COLORS [ 16 ];

extern void sdlHelper_exit();
extern void sdlHelper_setup();

extern uint8_t scale_set( int8_t scale );
extern void    render();
extern void    tv_clear( int color );
extern void    pixel( int x, int y, int col );
extern void    tv_char( int x, int y, const int ch, int col );
extern void    tv_print( int x, int y, int col, char* fmt, ... );
// extern void text(int x, int y, int* ch, int col);
extern void line( int x0, int y0, int x1, int y1, int col );
extern void lineX( int x, int col );
extern void lineY( int y, int col );
extern void border( uint16_t border_x, uint16_t border_y, uint16_t border_width,
                       uint16_t border_height, uint8_t border_type, uint8_t color );

extern void     rect( int x, int y, int w, int h, int col, int filled );
extern uint16_t key_pressed( uint16_t compairBtns );
extern uint16_t key_released( uint16_t compairBtns );
extern void     key_poll();
extern void     keys_clear();
