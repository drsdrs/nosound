#include <sys/resource.h>

uint64_t deltaAvg;
uint64_t startMem;
int      i = 0;

Sprite *spriteCollection[16];

uint64_t prg_memory_usage() {
    static struct rusage r_usage;
    getrusage( RUSAGE_SELF, &r_usage );
    return r_usage.ru_maxrss;
}

void setup() {
    tv_clear( 1 );
    tv_scale_set( 2 );
    loop_interval_set( 0 );
    startMem                = prg_memory_usage();
    int texturePositions[8] = { 0, 0, 0, 8, 0, 16, 0, 24 };
    for ( i = 0; i < 16; i++ ) {
        spriteCollection[i] = spriteManager_get( i );
    }
}

void loop() {
    spriteManager_draw(
        spriteCollection[0],
        vect2_new( ( loop_lifetime_ns / 40000000 / 2 ) % WIDTH, HEIGHT2 - 8 ),
        ( loop_lifetime_ns / 40000000 ) & 0b11 );
    spriteManager_draw( spriteCollection[1], vect2_new( ( loop_lifetime_ns / 100000000 / 4 ) % WIDTH, HEIGHT2 ), ( loop_lifetime_ns / 100000000 ) & 0b11 );
    spriteManager_draw( spriteCollection[2], vect2_new( ( loop_lifetime_ns / 150000000 / 4 ) % WIDTH, HEIGHT2 + 24 ), ( loop_lifetime_ns / 150000000 ) & 0b11 );
    spriteManager_draw( spriteCollection[3], vect2_new( ( loop_lifetime_ns / 45000000 / 2 ) % WIDTH, HEIGHT2 + 16 ), ( loop_lifetime_ns / 145000000 ) & 0b11 );
    spriteManager_draw( spriteCollection[4], vect2_new( ( loop_lifetime_ns / 80000000 / 2 ) % WIDTH, HEIGHT2 + 32 ), ( loop_lifetime_ns / 180000000 ) & 0b11 );
    spriteManager_draw( spriteCollection[5], vect2_new( ( loop_lifetime_ns / 85000000 / 2 ) % WIDTH, HEIGHT2 - 16 ), ( loop_lifetime_ns / 185000000 ) & 0b11 );
    spriteManager_draw( spriteCollection[6], vect2_new( ( loop_lifetime_ns / 120000000 / 2 ) % WIDTH, HEIGHT2 - 24 ), ( loop_lifetime_ns / 120000000 ) & 0b11 );
    spriteManager_draw( spriteCollection[7], vect2_new( ( loop_lifetime_ns / 115000000 / 2 ) % WIDTH, HEIGHT2 + 48 ), ( loop_lifetime_ns / 15000000 ) & 0b11 );

    spriteManager_draw( spriteCollection[8], vect2_new( ( loop_lifetime_ns / 14660000 / 2 ) % WIDTH, HEIGHT2 - 8 ), ( loop_lifetime_ns / 40000000 ) & 0b11 );
    spriteManager_draw( spriteCollection[9], vect2_new( ( loop_lifetime_ns / 11880000 / 4 ) % WIDTH, HEIGHT2 ), ( loop_lifetime_ns / 100000000 ) & 0b11 );
    spriteManager_draw( spriteCollection[0xa], vect2_new( ( loop_lifetime_ns / 156600000 / 4 ) % WIDTH, HEIGHT2 + 24 ), ( loop_lifetime_ns / 150000000 ) & 0b11 );
    spriteManager_draw( spriteCollection[0xb], vect2_new( ( loop_lifetime_ns / 145444000 / 2 ) % WIDTH, HEIGHT2 + 16 ), ( loop_lifetime_ns / 45000000 ) & 0b11 );
    spriteManager_draw( spriteCollection[0xc], vect2_new( ( loop_lifetime_ns / 187700000 / 2 ) % WIDTH, HEIGHT2 + 32 ), ( loop_lifetime_ns / 80000000 ) & 0b11 );
    spriteManager_draw( spriteCollection[0xd], vect2_new( ( loop_lifetime_ns / 185880000 / 2 ) % WIDTH, HEIGHT2 - 16 ), ( loop_lifetime_ns / 85000000 ) & 0b11 );
    spriteManager_draw( spriteCollection[0xe], vect2_new( ( loop_lifetime_ns / 112990000 / 2 ) % WIDTH, HEIGHT2 - 24 ), ( loop_lifetime_ns / 20000000 ) & 0b11 );
    spriteManager_draw( spriteCollection[0xf], vect2_new( ( loop_lifetime_ns / 115550000 / 2 ) % WIDTH, HEIGHT2 + 48 ), ( loop_lifetime_ns / 15000000 ) & 0b11 );

    deltaAvg = deltaAvg * 0.99f + loop_delta_full_ns * 0.01f;

    //// make PRG_debug out of this -->
    tv_print( WIDTH / 2, HEIGHT / 12 * 2, 0xfff, "Memory %.3f Mb\n", (float)prg_memory_usage() / 1000 );
    tv_print( WIDTH / 2, HEIGHT / 12 * 3, 0xfff, "fps: %.3f", 1 / ( (float)loop_delta_full_ns / 1000000000 ) );
    // tv_print( WIDTH / 2, HEIGHT / 8 * 5, 0xfff, "Mem: %f Mb\n", (float)prg_memory_usage() / 1000
    // );
    // tv_print( WIDTH / 2, HEIGHT / 8 * 7, 0xfff, "fps: %f ", 1 / ( deltaAvg /
    //  1000000000.0f ) );
}
