#pragma once

#include <stdint.h>

#include "Vect2.h"

typedef struct Sprite {
    char    *name;
    Vect2    size;
    uint16_t frames;
    int     *texture_positions;
} Sprite;

extern Sprite  sprite_new( char *name, int width, int height, int frames, int *texture_positions );
extern void    spriteManager_setup();
extern Sprite *spriteManager_find( char *spriteName );
extern Sprite *spriteManager_get( int spriteNr );
extern void    spriteManager_draw( Sprite *sprite, Vect2 drawPos, uint16_t frame );
