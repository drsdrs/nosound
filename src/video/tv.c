#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>

#include "SDL2_inprint.h"
#include "loopManager.h"
#include "prg.h"
#include "tv.h"

SDL_Window*   window;
SDL_Surface*  tv;
SDL_Surface*  pixels;
SDL_Renderer* renderer;
SDL_Joystick* gamepad;

uint16_t RES_W        = 640;
uint16_t RES_H        = 480;
uint16_t SOURCE_RES_W = 640;
uint16_t SOURCE_RES_H = 480;

uint8_t CHAR_HEIGHT  = 8;
uint8_t CHAR_HEIGHT2 = 4;

uint16_t WIDTH, WIDTH2;
uint16_t HEIGHT, HEIGHT2;
uint8_t  SCALE;

uint8_t ROWS;
uint8_t LINES;

const int JOYSTICK_DEAD_ZONE = 8000;

void tv_exit() {
    static uint8_t exit_flag = false;
    if ( exit_flag == true ) return;
    exit_flag = true;
    printf( "TV_Exit\n" );
    SDL_JoystickClose( gamepad );
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    SDL_Quit();
    kill_inline_font();
    // loop_exit();
}

void setColor( int col ) {
    if ( col == -1 ) return;
    SDL_SetRenderDrawColor( renderer,
                            ( ( col >> 8 ) & 0xf ) * 17,
                            ( ( col >> 4 ) & 0xf ) * 17,
                            ( col & 0xf ) * 17,
                            255 );
}

void tv_clear( int col ) {
    SDL_SetRenderDrawColor( renderer, ( ( col >> 8 ) & 0xf ) * 17, ( ( col >> 4 ) & 0xf ) * 17,
                            ( col & 0xf ) * 17, 255 );
    SDL_RenderClear( renderer );
}

void tv_render() {
    SDL_RenderPresent( renderer );
}

void tv_pixel( int x, int y, int col ) {
    setColor( col );
    SDL_RenderDrawPoint( renderer, x, y );
}

void tv_rect( int x, int y, int w, int h, int col, int filled ) {
    static SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.h = h;
    rect.w = w;
    setColor( col );
    if ( filled ) {
        SDL_RenderFillRect( renderer, &rect );
    } else {
        SDL_RenderDrawRect( renderer, &rect );
    }
}

void tv_line( int x0, int y0, int x1, int y1, int col ) {
    setColor( col );
    SDL_RenderDrawLine( renderer, x0, y0, x1, y1 );
}
void tv_lineX( int x, int col ) {
    setColor( col );
    SDL_RenderDrawLine( renderer, x, 0, x, HEIGHT );
}
void tv_lineY( int y, int col ) {
    setColor( col );
    SDL_RenderDrawLine( renderer, 0, y, WIDTH, y );
}

void tv_char( int x, int y, int ch, int col ) {
    int color = ( ( ( ( col >> 8 ) & 0xf ) * 17 ) << 16 ) +
                ( ( ( ( col >> 4 ) & 0xf ) * 17 ) << 8 ) + ( ( col & 0xf ) * 17 );
    // printf("Color: %3x %i-%i-%i \n", color, x, y, col);
    incolor( color, /* unused */ 0 );
    inprint_char( renderer, ch, x, y );
}

void tv_print( int x, int y, int col, char* fmt, ... ) {
    char*     str        = malloc( sizeof( str ) * 80 * 60 );
    const int strHeight2 = 4 - 1;
    const int color      = ( ( ( ( col >> 8 ) & 0xf ) * 17 ) << 16 ) + ( ( ( ( col >> 4 ) & 0xf ) * 17 ) << 8 ) + ( ( col & 0xf ) * 17 );
    va_list   va;
    va_start( va, fmt );
    vsprintf( str, fmt, va );
    va_end( va );
    incolor( color, /* unused */ 0 );
    const int strWidth2 = ( strlen( str ) * 4 ) - 1;
    inprint( renderer, str, x - strWidth2, y - strHeight2 );
    free( str );
}

void tv_border( uint16_t border_x, uint16_t border_y, uint16_t border_width, uint16_t border_height, uint8_t border_type, uint8_t color ) {
    char symbol;
    border_width--;
    border_height--;
    for ( size_t x = 0; x <= 1; x++ ) {
        for ( size_t y = 1; y < border_height; y++ ) {
            if ( x == 0 ) {
                symbol = ASCII_BORDER_LEFT;
            } else if ( x == 1 ) {
                symbol = ASCII_BORDER_RIGHT;
            }
            tv_char( border_x + x * border_width * 8, border_y + y * 8, symbol, color );
        }
    }
    for ( size_t x = 1; x < border_width; x++ ) {
        for ( size_t y = 0; y <= 1; y++ ) {
            if ( y == 0 ) {
                symbol = ASCII_BORDER_TOP;
            } else if ( y == 1 ) {
                symbol = ASCII_BORDER_BOTTOM;
            }
            tv_char( border_x + x * 8, border_y + y * border_height * 8, symbol, color );
        }
    }

    if ( border_type ) {
        symbol = ASCII_BORDER_ROUND_TOP_LEFT;
    } else {
        symbol = ASCII_BORDER_TOP_LEFT;
    }
    tv_char( border_x, border_y, symbol, color );

    if ( border_type ) {
        tv_char( border_x + (border_width)*8, border_y - 8, ASCII_BORDER_ROUND_TOP_RIGHT, color );
    } else {
        tv_char( border_x + (border_width)*8, border_y, ASCII_BORDER_TOP_RIGHT, color );
    }

    if ( border_type ) {
        symbol = ASCII_BORDER_ROUND_BOTTOM_LEFT;
    } else {
        symbol = ASCII_BORDER_BOTTOM_LEFT;
    }
    tv_char( border_x, border_y + border_height * 8, symbol, color );

    if ( border_type ) {
        symbol = ASCII_BORDER_ROUND_BOTTOM_RIGHT;
    } else {
        symbol = ASCII_BORDER_BOTTOM_RIGHT;
    }
    tv_char( border_x + (border_width)*8, border_y + border_height * 8, symbol, color );
}

/*		TODO move up if can*/
#include "../input/keys.c"

uint8_t tv_scale_set( int8_t scale ) {
    if ( scale > 5 || scale < 0 ) {
        printf( "Wrong scale: %i\n", scale );
        return 0;
    }
    // printf("setScale: %i\n", SCALE);
    SCALE = 1 << scale;
    SDL_RenderSetScale( renderer, SCALE, SCALE );

    WIDTH   = SOURCE_RES_W / SCALE;
    HEIGHT  = SOURCE_RES_H / SCALE;
    HEIGHT2 = HEIGHT / 2;
    WIDTH2  = WIDTH / 2;
    ROWS    = WIDTH / 8;
    LINES   = HEIGHT / 8;
    return 1;
}

void initWindow( uint8_t scale ) {
    RES_W *= 1;
    RES_H *= 1;

    window   = SDL_CreateWindow( "SDL pixels", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                 RES_W, RES_H, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
    tv       = SDL_GetWindowSurface( window );
    tv_scale_set( scale );
}

void tv_setup() {
    if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK ) < 0 ) {
        printf(
            "SDL could not be initialized!\n"
            "SDL_Error: %s\n",
            SDL_GetError() );
    }

    initWindow( 1 );

    gamepad = SDL_JoystickOpen( 0 );

    inrenderer( renderer );
    prepare_inline_font();
}
