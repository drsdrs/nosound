#pragma once

typedef struct {
  uint16_t x;
  uint16_t y;
} Vect2;

extern Vect2 vect2_new( uint16_t x, uint16_t y );

extern void vect2_add( Vect2 *srcPointer, Vect2 *trgPointer );
