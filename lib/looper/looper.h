#pragma once

#include <stdbool.h>


#define MEASURE_START 0
#define MEASURE_STOP 1
#define MEASURE_STOP_AVG 2

extern uint64_t loop_millis;
extern uint64_t loop_frames;
extern uint32_t loop_interval_us;
extern void loop_exit();
extern void loop_sleep( uint32_t sleep_ms );
extern void loop_interval_set( uint32_t new_interval );
extern void loop_setup_PRIVATE( void(* setup_funct)(), void(* loop_funct)(), uint32_t new_interval );
extern long int loop_measure_ns( uint8_t start_end );
