#include <sys/resource.h>

#include "../../src/classes/Entity.c"

uint64_t deltaAvg;
uint64_t startMem;
int      i = 0;


//		TODO		fuck it, use images as spritessheets
// ONE default size for spritesheet, like 512/1024
// one spritesheet per project, "sprites.png"
// convert all colors to color12..... alpha???
uint8_t imgData [ 3 + 16 ] = {    //
    8,                            // WIDTH
    8,                            // HEIGHT
    2,                            // FRAMES
    0b00111100,                   //
    0b01111110,                   //
    0b01110110,                   //
    0b11111111,                   //
    0b11111111,                   //
    0b01111110,                   //
    0b01111110,                   //
    0b00111100,                   //

    0b00111100,                   //
    0b01110110,                   //
    0b01111111,                   //
    0b11111000,                   //
    0b11110000,                   //
    0b01111111,                   //
    0b01111110,                   //
    0b00111100,                   //
     };

uint8_t imgData1 [ 3 + 8 ] = { 16,         4,          1,          0b11111111,
                               0b11111111, 0b11000000, 0b00000000, 0b11000000,
                               0b00000000, 0b11111111, 0b11111111 };

Entity entity;

uint64_t prg_memory_usage() {
    static struct rusage r_usage;
    getrusage( RUSAGE_SELF, &r_usage );
    return r_usage.ru_maxrss;
}

void setup() {
    tv_clear( 1 );
    tv_scale_set( 2 );
    loop_interval_set( 1000000 / 9 );
    entity    = Entity_new( 0, vect2_new( 2, 2 ), vect2_new( 1, 1 ), imgData );
    Entity e1 = Entity_new( 0, vect2_new( 3, 3 ), vect2_new(), imgData );
    Entity_collection_add( entity );
    Entity_collection_add( e1 );
    startMem = prg_memory_usage();
}

void loop() {
    // Entity_draw( &entity );
    // Entity_move( &entity );
    Entity_collection_act_all();
    deltaAvg = deltaAvg * 0.99f + loop_delta_full_ns * 0.01f;

    // make PRG_debug out of this -->
    tv_print( WIDTH / 2, HEIGHT / 8 * 3, 0xfff, "lifetime: %2.2f s",
              (float)loop_lifetime_ns / 1000000000 );
    tv_print( WIDTH / 2, HEIGHT / 8 * 4, 0xfff, "delta: %7i us", deltaAvg / 1000 );
    tv_print( WIDTH / 2, HEIGHT / 8 * 5, 0xfff, "Mem: %f Mb\n", (float)prg_memory_usage() / 1000 );
    tv_print( WIDTH / 2, HEIGHT / 8 * 6, 0xfff, "MemNEW: %ld kb\n", prg_memory_usage() - startMem );
    tv_print( WIDTH / 2, HEIGHT / 8 * 7, 0xfff, "fps: %f ", 1 / ( deltaAvg / 1000000000.0f ) );

    // struct timespec time;
    // clock_gettime(CLOCK_MONOTONIC_RAW, &time);
    // double number_of_processors = sysconf(_SC_NPROCESSORS_ONLN);
    // tv_print(
    // WIDTH/2, HEIGHT/8*7, 0xfff,
    //"CPU: %f %\n", ((double)clock()/number_of_processors/(loop_lifetime_ns/1000000+1))
    //);

    // for (int i = 1; i < 0xff; ++i) {
    // for (int u = 1; u < 0xff; ++u){
    // char* doing = malloc(100000);
    // int a = 99999/u*i/99999*sin(324234)*sin(234234);
    // free(doing);
    //}
    //}
}
