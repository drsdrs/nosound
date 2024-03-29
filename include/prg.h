#pragma once
#include <stdint.h>

extern void fps_set( double fps );
extern double fps_get();
extern const char* prg_name_get();
extern void        prg_change( char* prgName );
extern void        prg_data_save( uint16_t pos, uint8_t val );
extern uint8_t     prg_data_load( uint16_t pos );
extern uint64_t prg_memory_get();
