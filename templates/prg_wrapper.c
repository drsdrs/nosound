#include "loopManager.h"
#include "spriteManager.h"
#include "sdlHelper.h"
#include "beeper.h"
#include "prg.h"

#include "../main.c"

int main( int argc, char const *argv [] ) {
    loop_setup_PRIVATE( setup, loop, loop_interval_us );
}
