/**
 *  @file spriteManager.h
 *  @brief All about sprites.
 */

#pragma once

#include <stdint.h>

#include "Vect2.h"

/** @brief Sprite definition */
typedef struct Sprite {
    char    *name;
    Vect2    size;
    uint16_t frames; /** Amount of frames  */
    int     *texture_positions; /** Frame positions on the spritesheet.png */
} Sprite;

/** get a new instance of a Sprite */
extern Sprite  spriteManager_new( char *name, int width, int height, int frames, int *texture_positions );
/** common setup function */
extern void    spriteManager_setup();
/** Find sprite by name */
extern Sprite *spriteManager_find( char *spriteName );
/** Get sprite by number */
extern Sprite *spriteManager_get( int spriteNr );
/** Draw sprite */
extern void    spriteManager_draw( Sprite *sprite, Vect2 drawPos, uint16_t frame );
