#include "loopManager.h"
#include "spriteManager.h"
#include "tv.h"
#include "prg.h"

#include "../main.c"

int main( int argc, char const *argv [] ) {
    loop_setup_PRIVATE( setup, loop, loop_interval_us );
}
