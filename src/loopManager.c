#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "beeper.h"

#include "loopManager.h"
#include "spriteManager.h"
#include "prgManager.h"
#include "sdlHelper.h"
#include "waves.h"

uint64_t loop_delta_full_ns;
uint64_t loop_lifetime_ns;
uint64_t loop_frames;
uint32_t loop_interval_us = 1000000 / 20;

uint64_t loop_delta_ns;
int64_t  diff_ns;
int64_t  diff_ns_averaged;
uint64_t slept_ns;

uint8_t loop_quit;

void loop_sleep_ns( int64_t sleep_ns ) {
    static struct timespec start, end;
    uint32_t               slept_ns_temp = 0;
    clock_gettime( CLOCK_MONOTONIC_RAW, &start );
    while ( slept_ns_temp < sleep_ns ) {    // ~ 55 us 1 cycle
                                            // printf("slept_ns_temp %ins\n", slept_ns_temp);
        // for (size_t i = 0; i < 18; i++) {
        key_poll();
        SDL_Delay( 0 );
        //}
        clock_gettime( CLOCK_MONOTONIC_RAW, &end );
        slept_ns_temp =
            ( end.tv_sec - start.tv_sec ) * 1000000000 + ( end.tv_nsec - start.tv_nsec );
    }

    slept_ns += slept_ns_temp;
}

void loop_sleep_us( int64_t sleep_us ) {
    loop_sleep_ns( sleep_us * 1000 );
}

void loop_sleep_ms( int64_t sleep_ms ) {
    loop_sleep_ns( sleep_ms * 1000000 );
}

void loop_sleep( int64_t sleep_s ) {
    loop_sleep_ns( sleep_s * 1000000000 );
}

double loop_delta_get() {
    return ( ( loop_delta_full_ns ) / 1000000000.0f );
}

void loop_loop( void ( *loop_funct )() ) {
    static struct timespec start, end;

    clock_gettime( CLOCK_MONOTONIC_RAW, &start );

    slept_ns = 0;

    tv_clear( 0 );
    loop_funct();
    render();

    keys_clear();
    key_poll();
    loop_frames++;

    clock_gettime( CLOCK_MONOTONIC_RAW, &end );

    loop_delta_ns = ( end.tv_sec - start.tv_sec ) * 1000000000 + ( end.tv_nsec - start.tv_nsec );

    diff_ns = ( ( loop_interval_us * 1000 ) + slept_ns ) - ( loop_delta_ns );

    if ( diff_ns < 0 ) {    //
                            // printf("Loop needs too much time, overrun us:%li\n", diff_ns );
    } else {
        // printf("bev sleep\n");
        // printf("aft sleep\n");
    }

    loop_sleep_ns( diff_ns );

    loop_delta_full_ns = loop_delta_ns + slept_ns;
    loop_lifetime_ns += loop_delta_full_ns;
    diff_ns_averaged = ( diff_ns_averaged + diff_ns ) >> 1;
}

void loop_exit() {
    printf( "void loop_exit()\n\n" );
    loop_quit = true;
    Beeper_exit();
    sdlHelper_exit();
}

void loop_interval_set( int64_t new_interval ) {
    loop_interval_us = new_interval;
}

long int loop_measure_us( uint8_t start_end ) {
    static struct timespec start, end;
    static long int        avg = -1;
    long int               res;

    if ( start_end == MEASURE_START ) {
        clock_gettime( CLOCK_MONOTONIC_RAW, &end );
        res = 0;
        // avg = -1;
    } else if ( start_end == MEASURE_STOP || start_end == MEASURE_STOP_AVG ) {
        start = end;
        clock_gettime( CLOCK_MONOTONIC_RAW, &end );
        res = ( end.tv_sec - start.tv_sec ) * 1000000 + ( end.tv_nsec - start.tv_nsec ) / 1000;
    }

    if ( start_end == MEASURE_STOP_AVG ) {
        if ( avg == -1 ) {
            avg = res;
        } else {
            res = avg * .98f + res * .02f;
            avg = res;
        }
    }

    return res;
}

void loop_setup_PRIVATE( void ( *setup_funct )(), void ( *loop_funct )(), uint32_t new_interval ) {
    printf( "Start loop_setup_PRIVATE\n" );
    loop_frames      = 0;
    loop_interval_us = new_interval;

    loop_measure_us( MEASURE_START );

    sdlHelper_setup();
    printf( "sdlHelper_setup took: %li us\n", loop_measure_us( MEASURE_STOP ) );

    Beeper_setup();
    printf( "Beeper_setup took: %li us\n", loop_measure_us( MEASURE_STOP ) );

    waves_all_generate();
    printf( "waves_all_generate took: %li us\n", loop_measure_us( MEASURE_STOP ) );

    spriteManager_setup();
    printf( "spriteManager_setup took: %li us\n", loop_measure_us( MEASURE_STOP ) );

    setup_funct();
    printf( "PRG setup function took: %li us\n", loop_measure_us( MEASURE_STOP ) );


    tv_clear( 0 );
    while ( loop_quit == false ) loop_loop( loop_funct );

    printf( "Final PRG exit in loop_setup_PRIVATE\n" );
    exit( 0 );
}
