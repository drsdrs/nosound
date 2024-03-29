#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>

#include "SDL2_inprint.h"
#include "loopManager.h"
#include "prg.h"
#include "sdlHelper.h"

SDL_Window*   window;
SDL_Surface*  tv;
SDL_Surface*  pixels;
SDL_Renderer* renderer;
SDL_Joystick* gamepad;

#define RES_W 800
#define RES_H 800
uint16_t SOURCE_RES_W = RES_W;
uint16_t SOURCE_RES_H = RES_H;

uint8_t CHAR_HEIGHT  = 8;
uint8_t CHAR_HEIGHT2 = 4;

uint16_t WIDTH, WIDTH2;
uint16_t HEIGHT, HEIGHT2;
uint8_t  SCALE;

uint8_t ROWS;
uint8_t LINES;

char tempPrintString[80*60];

const int JOYSTICK_DEAD_ZONE = 8000;

uint16_t lastBtnsPressed;
uint16_t lastBtnsReleased;

uint16_t key_pressed( uint16_t compairBtns ) {
    if ( compairBtns == 0 ) return lastBtnsPressed;
    return ( compairBtns == lastBtnsPressed );
}

uint16_t key_released( uint16_t compairBtns ) {
    if ( compairBtns == 0 ) return lastBtnsReleased;
    return ( compairBtns == lastBtnsReleased );
}

void keys_clear() {
    lastBtnsPressed  = 0;
    lastBtnsReleased = 0;
}

uint16_t key_wait() {
    while ( lastBtnsPressed == 0 || lastBtnsReleased == 0 ) loop_sleep_us( 1 );
    if ( lastBtnsPressed != 0 ) return lastBtnsPressed;
    if ( lastBtnsReleased != 0 ) return lastBtnsReleased;
}

void key_poll() {
    static uint16_t tempBtnValues;
    SDL_Event       e;
    if ( SDL_PollEvent( &e ) == 0 ) return;

    if ( e.type == SDL_QUIT ) {
        printf( "e.type == SDL_QUIT\n" );
        loop_exit();

    } else if ( e.type == SDL_KEYDOWN || e.type == SDL_KEYUP ) {
        static uint16_t lastKeyboardKey;
        SDL_Keymod      key_mod = SDL_GetModState() & 4095;
        tempBtnValues           = 0;
        // printf("key pressed %i - %c\n", e.key.keysym.sym ,e.key.keysym.sym );
        if ( e.key.keysym.sym == 'w' ) {
            tempBtnValues = BTN_UP;
        } else if ( e.key.keysym.sym == 's' ) {
            tempBtnValues = BTN_DOWN;
        } else if ( e.key.keysym.sym == 'd' ) {
            tempBtnValues = BTN_RIGHT;
        } else if ( e.key.keysym.sym == 'a' ) {
            tempBtnValues = BTN_LEFT;
        } else if ( e.key.keysym.sym == ' ' ) {
            tempBtnValues = BTN_A;
        } else if ( key_mod == KMOD_LSHIFT && e.key.keysym.sym == 27 ) {
            printf( "Quit key SHIFT+ESC\n" );
            loop_exit();
            // exit(0);
        } else if ( key_mod == KMOD_LCTRL && e.key.keysym.sym == 27 ) {
            printf( "GOTO prgManagerUI CTRL+ESC\n" );
            loop_exit();
            prg_change( "program_starter" );
        }

        if ( e.type == SDL_KEYDOWN && lastKeyboardKey != tempBtnValues ) {
            lastKeyboardKey = tempBtnValues;
            lastBtnsPressed |= tempBtnValues;
        } else if ( e.type == SDL_KEYUP ) {
            lastKeyboardKey = 0;
            lastBtnsReleased |= tempBtnValues;
        }

        // SDL_GetModState()-4096;
    } else if ( e.type == SDL_JOYBUTTONDOWN || e.type == SDL_JOYBUTTONUP ) {
        tempBtnValues = 0;
        if ( SDL_JoystickGetButton( gamepad, 0 ) ) {
            tempBtnValues = BTN_A;
        } else if ( SDL_JoystickGetButton( gamepad, 1 ) ) {
            tempBtnValues = BTN_B;
        } else if ( SDL_JoystickGetButton( gamepad, 2 ) ) {
            tempBtnValues = BTN_X;
        } else if ( SDL_JoystickGetButton( gamepad, 3 ) ) {
            tempBtnValues = BTN_Y;
        } else if ( SDL_JoystickGetButton( gamepad, 4 ) ) {
            tempBtnValues = BTN_S_LEFT;
        } else if ( SDL_JoystickGetButton( gamepad, 5 ) ) {
            tempBtnValues = BTN_S_RIGHT;
        } else if ( SDL_JoystickGetButton( gamepad, 6 ) ) {
            tempBtnValues = BTN_SELECT;
        } else if ( SDL_JoystickGetButton( gamepad, 7 ) ) {
            tempBtnValues = BTN_START;
        }

        if ( e.type == SDL_JOYBUTTONDOWN ) {
            lastBtnsPressed |= tempBtnValues;
        } else if ( e.type == SDL_JOYBUTTONUP ) {
            lastBtnsReleased |= tempBtnValues;
        }

    } else if ( e.type == SDL_JOYAXISMOTION ) {    // Motion on controller 0
        static uint8_t lastDirX = 0;
        static uint8_t lastDirY = 0;
        if ( e.jaxis.which == 0 ) {
            if ( e.jaxis.axis == 0 ) {    //  LEFT - RIGHT
                if ( e.jaxis.value < -JOYSTICK_DEAD_ZONE ) {
                    lastBtnsPressed = BTN_LEFT;
                    lastDirX        = BTN_LEFT;
                } else if ( e.jaxis.value > JOYSTICK_DEAD_ZONE ) {
                    lastBtnsPressed = BTN_RIGHT;
                    lastDirX        = BTN_RIGHT;
                } else {
                    lastBtnsReleased = lastDirX;
                    lastDirX         = 0;
                }
            } else if ( e.jaxis.axis == 1 ) {                   // UP - DOWN
                if ( e.jaxis.value < -JOYSTICK_DEAD_ZONE ) {    // Below of dead zone
                    lastBtnsPressed = BTN_UP;
                    lastDirY        = BTN_UP;
                } else if ( e.jaxis.value > JOYSTICK_DEAD_ZONE ) {
                    lastBtnsPressed = BTN_DOWN;
                    lastDirY        = BTN_DOWN;
                } else {
                    lastBtnsReleased |= lastDirY;
                    lastDirY = 0;
                }
            }
        }
    }
}


void sdlHelper_exit() {
	printf("sdlHelper_exit()\n");
    kill_inline_font();
    SDL_DestroyWindow( window );
    SDL_JoystickClose( gamepad );
    SDL_DestroyRenderer( renderer );
    SDL_Quit();
}

void setColor( int col ) {
    if ( col < 0 ) return;
    SDL_SetRenderDrawColor( renderer,
                            ( ( col >> 8 ) & 0xf ) * 17,    //  red
                            ( ( col >> 4 ) & 0xf ) * 17,    //  green
                            ( col & 0xf ) * 17,             //  blue
                            255 );                          //  alpha
}

void tv_clear( int col ) {
    setColor( col );
    SDL_RenderClear( renderer );
}

void render() {
    SDL_RenderPresent( renderer );
}

void pixel( int x, int y, int col ) {
    setColor( col );
    SDL_RenderDrawPoint( renderer, x, y );
}

void rect( int x, int y, int w, int h, int col, int filled ) {
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

void line( int x0, int y0, int x1, int y1, int col ) {
    setColor( col );
    SDL_RenderDrawLine( renderer, x0, y0, x1, y1 );
}
void lineX( int x, int col ) {
    setColor( col );
    SDL_RenderDrawLine( renderer, x, 0, x, HEIGHT );
}
void lineY( int y, int col ) {
    setColor( col );
    SDL_RenderDrawLine( renderer, 0, y, WIDTH, y );
}

void tv_char( int x, int y, int ch, int col ) {
    int color = ( ( ( ( col >> 8 ) & 0xf ) * 17 ) << 16 ) +
                ( ( ( ( col >> 4 ) & 0xf ) * 17 ) << 8 ) + ( ( col & 0xf ) * 17 );
    incolor( color, /* unused */ 0 );
    inprint_char( renderer, ch, x, y );
}

void print( int x, int y, int col, char* fmt, ... ) {
    const int color      = ( ( ( ( col >> 8 ) & 0xf ) * 17 ) << 16 ) + ( ( ( ( col >> 4 ) & 0xf ) * 17 ) << 8 ) + ( ( col & 0xf ) * 17 );
    va_list   va;
    va_start( va, fmt );
    vsprintf( tempPrintString, fmt, va );
    va_end( va );
    incolor( color, /* unused */ 0 );
    inprint( renderer, tempPrintString, x, y );

}
void print_center( int x, int y, int col, char* fmt, ... ) {
    const int strHeight2 = 4 - 1;
    const int color      = ( ( ( ( col >> 8 ) & 0xf ) * 17 ) << 16 ) + ( ( ( ( col >> 4 ) & 0xf ) * 17 ) << 8 ) + ( ( col & 0xf ) * 17 );
    va_list   va;
    va_start( va, fmt );
    vsprintf( tempPrintString, fmt, va );
    va_end( va );
    incolor( color, /* unused */ 0 );
    const int strWidth2 = ( strlen( tempPrintString ) * 4 ) - 1;
    inprint( renderer, tempPrintString, x - strWidth2, y - strHeight2 );
}

void border( uint16_t border_x, uint16_t border_y, uint16_t border_width, uint16_t border_height, uint8_t border_type, uint8_t color ) {
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

uint8_t scale_set( int8_t scale ) {
    if ( scale > 5 || scale < 0 ) {
        printf( "Wrong scale: %i\n", scale );
        return 0;
    }
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
    //RES_W *= 1;
    //RES_H *= 1;

    window   = SDL_CreateWindow("SDL pixels", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, RES_W, RES_H, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
    tv       = SDL_GetWindowSurface( window );
    scale_set( scale );
}

void sdlHelper_setup() {
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
