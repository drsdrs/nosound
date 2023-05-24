#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

#include "prgManager.h"
#include "beeper.h"
#include "tv.h"
#include "looper.h"
#include "file.h"
#include "stringHelper.h"
#include "path.h"
#include "prg.h"
#include "vect.h"
#include "waves.h"
#include "color12.h"

#include "../main.c"

int main(int argc, char const *argv[]) {

  loop_setup_PRIVATE( setup, loop, loop_interval_us );

}
