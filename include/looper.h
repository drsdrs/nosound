#pragma once

#include <stdbool.h>

#define MEASURE_START    0
#define MEASURE_STOP     1
#define MEASURE_STOP_AVG 2

extern uint64_t loop_delta_full_ns;
extern uint64_t loop_lifetime_ns;
extern uint64_t loop_frames;
extern uint32_t loop_interval_us;
extern void     loop_exit();
extern double   loop_delta_get();
extern void     loop_sleep_ns( int64_t sleep_ns );
extern void     loop_sleep_us( int64_t sleep_us );
extern void     loop_sleep_ms( int64_t sleep_ms );
extern void     loop_sleep( int64_t sleep_s );

extern void     loop_interval_set( int64_t new_interval );
extern void     loop_setup_PRIVATE( void ( *setup_funct )(), void ( *loop_funct )(),
                                    uint32_t new_interval );
extern long int loop_measure_us( uint8_t start_end );
