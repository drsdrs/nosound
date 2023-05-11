#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "../lib/waves.h"

//#include "../lib/beeper/beeper.h"
//#include "../lib/tv/tv.h"
#include "../lib/looper/looper.h"
#include "../lib/path/path.h"
#include "../lib/prg/prg.h"
#include "../lib/prgManager/prgManager.h"

#include "../lib/term/term.h"


int main(int argc, char const *argv[]) {


  const char* startPrgName;
  if(argv[1]){
    startPrgName = argv[1];
  } else {
    startPrgName = "prgManagerUI";//argv[1];
  }
  const char* cleanup = argv[2];
  printf("Cleanup? %s\n", cleanup);
  prgManager_init( startPrgName, cleanup );
  // if( argc>1 ){
  //   prgManager_init( startPrgName );
  // } else {
  //   prgManager_init( NULL );
  //   printf("Nothing to start....\n");
  // }

  exit(1);
}
