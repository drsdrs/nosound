/**
 * @file nosound.c
 * @author drsdrs
 * @brief the engine that could
 *
 * more extensive explanation
 */

#include <dirent.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "path.h"
#include "prg.h"
#include "prgManager.h"
#include "term.h"


/** @brief Main function
 *
 * Parsing arguments from command-line,
 * starting prg_manager
 * */
int main( int argc, char const* argv[] ) {
    const char* startPrgName;
    if ( argv[1] != NULL && argc>0 ) {
        startPrgName = argv[1];
    } else {
        startPrgName = "program_starter";
    }
    const char* cleanup = NULL;    // argv[2];
    printf( "Cleanup? %s\n", cleanup );
    printf( "StartPRG %s\n", startPrgName );
    prgManager_init( startPrgName, cleanup );
    // if( argc>1 ){
    //   prgManager_init( startPrgName );
    // } else {
    //   prgManager_init( NULL );
    //   printf("Nothing to start....\n");
    // }

    exit( 1 );
}
