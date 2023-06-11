#include "term.h"

#include <stdint.h>
#include <stdio.h>

void term_newLine() {
    printf( "\n" );
}

void term_color_reset() {
    printf( "\033[0m" );
}

void term_color_set( uint8_t colorPairId ) {
    const char term_colorPairs [] = { 0x60, 0x50, 0x40, 0x30, 0x06, 0x05, 0x04, 0x03, 0x01, 0x10 };
    printf( "\033[3%i;4%im", ( term_colorPairs [ colorPairId ] >> 4 ) & 0xf,
            term_colorPairs [ colorPairId ] & 0xf );
}
