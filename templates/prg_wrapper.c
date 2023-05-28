#include <dirent.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "../main.c"
#include "Color12.h"
#include "Vect2.h"
#include "beeper.h"
#include "file.h"
#include "looper.h"
#include "path.h"
#include "prg.h"
#include "prgManager.h"
#include "stringHelper.h"
#include "tv.h"
#include "waves.h"

int main( int argc, char const *argv [] ) {
    loop_setup_PRIVATE( setup, loop, loop_interval_us );
}
