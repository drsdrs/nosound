#pragma once

#include <SDL2/SDL.h>

#include "tv_ASCII.h"
#include "tv_KEYS.h"
#include "tv_COLORS.h"

extern SDL_Renderer* renderer;

extern uint16_t WIDTH, WIDTH2;
extern uint16_t HEIGHT, HEIGHT2;
extern uint8_t CHAR_HEIGHT, CHAR_HEIGHT2;
extern uint8_t SCALE;

extern uint8_t ROWS;
extern uint8_t LINES;

extern uint16_t COLORS[16];

extern void tv_exit( );
extern void tv_setup( );

extern uint8_t tv_scale_set( int8_t scale );
extern void tv_render();
extern void tv_clear( int color );
extern void tv_pixel(int x, int y, int col);
extern void tv_char(int x, int y, const int ch, int col);
extern void tv_print(int x, int y, int col, char* fmt, ...);
//extern void text(int x, int y, int* ch, int col);
extern void tv_line(int x0, int y0, int x1, int y1, int col);
extern void tv_lineX(int x, int col);
extern void tv_lineY(int y, int col);
extern void tv_border( uint16_t border_x, uint16_t border_y, uint16_t border_width, uint16_t border_height, uint8_t border_type, uint8_t color );

extern void tv_rect(int x, int y, int w, int h, int col, int filled);
extern uint16_t tv_key_pressed( uint16_t compairBtns );
extern uint16_t tv_key_released( uint16_t compairBtns );
extern void tv_key_poll(  );
extern void tv_keys_clear(  );
