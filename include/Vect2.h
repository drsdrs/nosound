
/**
 * ********************************************
 *  @file vect.h
 *  @brief 2d vect
 *********************************************/

#pragma once
#include <stdint.h>

/**
 * @brief 2d vector struct
 */
typedef struct {
    uint16_t x;
    uint16_t y;
} Vect2;

/** Get vect2 from X and Y choordinates
 * @param x
 * @param b blue range 0-255
 * @return Vect2 */
extern Vect2 vect2_new( int16_t x, int16_t y );

/** Add target to source vect2
 * @param srcPointer
 * @param trgPointer
 */
extern void vect2_add( Vect2 *srcPointer, Vect2 *trgPointer );
