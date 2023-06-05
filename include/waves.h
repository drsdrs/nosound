#pragma once
#include <stdint.h>

extern void  waves_all_generate();
extern float waves_sine12_get( uint16_t i );
extern float waves_sine12_unbiased_get( uint16_t i );
extern float waves_sine8_get( uint8_t i );
extern float WAVES_SINE_8 [ 0x100 ];
extern float WAVES_SINE_12 [ 0x1000 ];
