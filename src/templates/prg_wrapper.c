#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

#include "../../../lib/prgManager/prgManager.h"
#include "../../../lib/beeper/beeper.h"
#include "../../../lib/tv/tv.h"
#include "../../../lib/looper/looper.h"
#include "../../../lib/file/file.h"
#include "../../../lib/string/string.h"
#include "../../../lib/path/path.h"
#include "../../../lib/prg/prg.h"
#include "../../../lib/vect/vect.h"
#include "../../../lib/waves.h"

#include "../main.c"

int main(int argc, char const *argv[]) {

  loop_setup_PRIVATE( setup, loop, loop_interval_us );

}
