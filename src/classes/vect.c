#include <stdint.h>

#include "vect.h"


void vect2_add( Vect2 *srcPointer, Vect2 *trgPointer ){
  srcPointer->x += trgPointer->x;
  srcPointer->y += trgPointer->y;
}

Vect2 vect2_new( uint16_t x, uint16_t y ){
  return (Vect2) { x, y };
}

Vect2 vect2_copy( Vect2 src ){
  return (Vect2) { src.x, src.y };
}
