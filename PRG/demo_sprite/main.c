#include <sys/resource.h>

uint64_t deltaAvg;
uint64_t startMem;
int      i = 0;

Sprite sprite;
Sprite sprite0;
Sprite sprite1;
Sprite sprite2;

uint64_t prg_memory_usage() {
    static struct rusage r_usage;
    getrusage( RUSAGE_SELF, &r_usage );
    return r_usage.ru_maxrss;
}

void setup() {
    tv_clear( 1 );
    tv_scale_set( 2 );
    loop_interval_set( 0 );
    startMem                   = prg_memory_usage();
    int texturePositions [ 8 ] = { 0, 0, 0, 8, 0, 16, 0, 24 };
    sprite                     = sprite_new( "Cow", 8, 8, 4, ( int [] ){ 0, 0, 0, 8, 0, 16, 0, 24 } );
    sprite0                    = sprite_new( "animal", 8, 8, 4, ( int [] ){ 8, 0, 8, 8, 8, 16, 8, 24 } );
    sprite1                    = sprite_new( "animal1", 8, 8, 4, ( int [] ){ 16, 0, 16, 8, 16, 16, 16, 24 } );
    sprite2                    = sprite_new( "animal2", 8, 8, 4, ( int [] ){ 24, 0, 24, 8, 24, 16, 24, 24 } );
}

void loop() {
    spriteManager_draw( &sprite0, vect2_new( ( loop_lifetime_ns / 40000000 / 2 ) % WIDTH, HEIGHT2 - 8 ), ( loop_lifetime_ns / 40000000 ) & 0b11 );
    spriteManager_draw( &sprite, vect2_new( ( loop_lifetime_ns / 100000000 / 4 ) % WIDTH, HEIGHT2 ), ( loop_lifetime_ns / 100000000 ) & 0b11 );
    spriteManager_draw( &sprite, vect2_new( ( loop_lifetime_ns / 150000000 / 4 ) % WIDTH, HEIGHT2 + 24 ), ( loop_lifetime_ns / 150000000 ) & 0b11 );
    spriteManager_draw( &sprite0, vect2_new( ( loop_lifetime_ns / 45000000 / 2 ) % WIDTH, HEIGHT2 + 16 ), ( loop_lifetime_ns / 45000000 ) & 0b11 );
    spriteManager_draw( &sprite1, vect2_new( ( loop_lifetime_ns / 80000000 / 2 ) % WIDTH, HEIGHT2 + 32 ), ( loop_lifetime_ns / 80000000 ) & 0b11 );
    spriteManager_draw( &sprite1, vect2_new( ( loop_lifetime_ns / 85000000 / 2 ) % WIDTH, HEIGHT2 - 16 ), ( loop_lifetime_ns / 85000000 ) & 0b11 );
    spriteManager_draw( &sprite2, vect2_new( ( loop_lifetime_ns / 20000000 / 2 ) % WIDTH, HEIGHT2 - 24 ), ( loop_lifetime_ns / 20000000 ) & 0b11 );
    spriteManager_draw( &sprite2, vect2_new( ( loop_lifetime_ns / 15000000 / 2 ) % WIDTH, HEIGHT2 + 48 ), ( loop_lifetime_ns / 15000000 ) & 0b11 );
    // deltaAvg = deltaAvg * 0.99f + loop_delta_full_ns * 0.01f;

    //// make PRG_debug out of this -->
    tv_print( WIDTH / 2, HEIGHT / 12 * 1, 0xfff, "spriteName: %s\n", sprite.name );
    tv_print( WIDTH / 2, HEIGHT / 12 * 2, 0xfff, "Memory %.3f Mb\n", (float)prg_memory_usage() / 1000 );
    tv_print( WIDTH / 2, HEIGHT / 12 * 3, 0xfff, "fps: %.3f", 1 / ( (float)loop_delta_full_ns / 1000000000 ) );
    // tv_print( WIDTH / 2, HEIGHT / 8 * 5, 0xfff, "Mem: %f Mb\n", (float)prg_memory_usage() / 1000
    // );
    // tv_print( WIDTH / 2, HEIGHT / 8 * 7, 0xfff, "fps: %f ", 1 / ( deltaAvg /
    //  1000000000.0f ) );
}
