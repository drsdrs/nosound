#include <dirent.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "file.h"
#include "path.h"
#include "prgManager.h"
#include "stringHelper.h"
#include "sys/wait.h"
#include "term.h"

int8_t   prgRunning = false;
char**   prgManager_list;
uint16_t prgManager_prgList_length = 0;
int      prgActivePid              = -1;
uint8_t  prgManager_watch_quit;

int runcmd( char* argv [], uint8_t waitFinish ) {
    pid_t child_pid;
    int   child_status;
    child_pid = fork();

    if ( child_pid == 0 ) {
        execvp( argv [ 0 ], argv );
        printf( "Unknown command // runcmd ERROR status: %i\t%s\n", child_status,
                argv [ 0 ] ); /* If execvp returns, it must have failed. */
        exit( 0 );
    }

    if ( waitFinish ) { /* This is run by the parent.  Wait for the child to terminate. */
        pid_t tpid = wait( &child_status );
        while ( tpid != child_pid ) {
            tpid = wait( &child_status );
            if ( tpid != child_pid ) {
                printf( "PRG quit!\n" );
            }    // process_terminated(tpid);
        };
        if ( child_status == -1 ) {
            printf( "Run cmd error status: %i\t%s\n", child_status, argv [ 0 ] );
            return -1;
        } else if ( child_status == 0 ) {
            printf( "Runcmd %s went OK! child_status:%i \n", argv [ 0 ], child_status );
        } else {
            term_color_set( 8 );
            printf( "Runcmd %s went bad, full of errors! child_status:%i \n", argv [ 0 ],
                    child_status );
            term_color_reset();
            term_newLine();
            return -1;
        }
    }
    return child_pid;
}

int runPrg( const char* prgName ) {
    char* cmdArgv [] = { path_binary_get( prgName ), NULL };
    int   pid        = runcmd( cmdArgv, false );
    return pid;
}

time_t file_timestamp_get( char* fileName ) {
    struct stat filestat;
    stat( fileName, &filestat );
    return filestat.st_ctime;
}

void prgManager_timestamp_compile_set( const char* prgName ) {
    file_write( path_timestamp_get( prgName ), "" );
}

uint8_t prgManager_timestamp_compile_changed( const char* prgName ) {
    static time_t stamp_saved, stamp_new;
    stamp_saved = file_timestamp_get( path_timestamp_get( prgName ) );
    stamp_new   = file_timestamp_get( path_source_get( prgName ) );
    // printf("Timestamp saved: %i \tnew: %i\n", stamp_saved, stamp_new);
    return stamp_new > stamp_saved;
}

uint8_t prgManager_compile( const char* prgName, int waitForProcess ) {
    term_newLine();
    term_color_set( 7 );
    printf( "prgManager_compile: %s", prgName );
    term_color_reset();
    term_newLine();

    folder_create( path_build_get( prgName ) );
    folder_create( path_assets_get( prgName ) );

    file_copy( "templates/prg_wrapper.c", path_wrapper_get( prgName ) );
    file_copy( "templates/CMakeLists.txt", path_cmakelist_get( prgName ) );

    //if ( !file_exist( path_spritesheet_get( prgName ) ) ) {
		file_copy( "templates/spritesheet.png", path_spritesheet_get( prgName ) );
		file_copy( "templates/spritesheet.json", path_spritesheetAtlas_get( prgName ) );
    //}

    if ( !file_exist( path_savefile_get( prgName ) ) ) {
        file_write( path_savefile_get( prgName ), "" );
    }

    char* argvCmake [] = { "cmake",
                           "-B",
                           path_build_get( prgName ),
                           "-S",
                           path_build_get( prgName ),
                           //"-D",
                           //"CMAKE_BUILD_TYPE=Debug",
                           "-G",
                           "Ninja",
                           NULL };

    char* argvMake [] = { "ninja", "-C", path_build_get( prgName ), NULL };
    if ( runcmd( argvCmake, true ) == -1 ) return false;
    if ( runcmd( argvMake, true ) == -1 ) return false;
    prgManager_timestamp_compile_set( prgName );

    return true;
}

void prgManager_run( const char* prgName ) {
    term_color_set( 5 );
    printf( "PRG: %s started", prgName );
    term_color_reset();
    term_newLine();
    prgActivePid = runPrg( prgName );
    prgRunning   = true;
}

void prgManager_activePrg_kill() {
    if ( prgRunning && prgActivePid != -1 ) {
        term_color_set( 8 );
        printf( "KILL running prg..." );
        kill( prgActivePid, SIGKILL );
        prgRunning   = false;
        prgActivePid = -1;
    } else if(prgActivePid!=-1) {
        printf( "\n***\n**\n*KILL running prg went wrong, prgRunning: %i prgActivePid: %i \n",
                prgRunning, prgActivePid );
    }
    term_color_reset();
    term_newLine();
}

// make common filewatcher for all files ???
int  watchCalls;
void prgManager_watch_exit() {
    printf( " prgManager_watch_exit   prgManager_watch_quit = %i; pointer: %hhn\n",
            prgManager_watch_quit, &prgManager_watch_quit );
    prgManager_watch_quit = true;
}

void prgManager_watch( const char* prgName ) {
    static uint8_t prgManager_watch_running = false;
    while ( prgManager_watch_running ) {
        prgManager_watch_quit = true;
        printf( "prgManager_watch_running -- -- --\n" );
    }
    prgManager_watch_running = true;
    uint8_t runOnceAtStart   = true;
    term_color_set( 6 );
    printf( "Watching: %s nr: %i", prgName, watchCalls );
    term_color_reset();
    term_newLine();
    while ( !prgManager_watch_quit ) {
        // printf( "while( !prgManager_watch_quit ){: %s, quit: %i watchCallPointer: %i \n", prgName
        // , prgManager_watch_quit, &watchCalls);
        if ( prgManager_timestamp_compile_changed( prgName ) || runOnceAtStart ) {
            runOnceAtStart = false;
            prgManager_activePrg_kill();
            term_color_set( 1 );
            printf( "Timestamp changed! Recompile and run: %s.\n", prgName );
            term_color_reset();
            if ( prgManager_compile( prgName, true ) == true ) {    // compiling went ok!
                prgManager_run( prgName );
            } else {
                printf( "\nCompiling went wrong due to error: %s.\n", prgName );
                prgManager_timestamp_compile_set( prgName );
            }
        }
        sleep( 1 );
    }
    printf( "Exit Watching: %s ", prgName );
    prgManager_activePrg_kill();
    // prgManager_watch_running = false;
}

void prgManager_list_get( char*** fileListPointer, uint16_t* fileListLengthPointer ) {
    char**         fileListTemp = malloc( sizeof( fileListTemp ) );
    DIR*           folder;
    struct dirent* entry;
    int            files = 0;
    folder               = opendir( "PRG" );
    if ( folder == NULL ) {
        printf( "prgManager_list_get ERROR Unable to read directory \n" );
        return;
    }

    while ( ( entry = readdir( folder ) ) ) {
        fileListTemp = realloc( fileListTemp, ( files + 1 ) * sizeof( fileListTemp ) );

        // if( folderName==NULL ){ continue; }    //    not a folder ...
        if ( entry->d_name [ 0 ] == '.' ) {
            continue;
        }    //    not a folder ...
        if ( entry->d_type != 4 ) {
            continue;
        }    //    not a folder ...

        fileListTemp [ files ] =
            malloc( sizeof( entry->d_name ) * sizeof( fileListTemp [ files ] ) );
        strcpy( fileListTemp [ files ], entry->d_name );
        // printf("Found folder nr: %i name:%s - %s\n", files, entry->d_name, fileListTemp[files] );
        files++;
    }
    *fileListLengthPointer = files;
    *fileListPointer       = fileListTemp;
    closedir( folder );
}

void prgManager_clean( const char* prgName ){

};

int prgManager_init( const char* startPrgName, const char* cleanup ) {
    prgManager_list_get( &prgManager_list, &prgManager_prgList_length );
    int         foundPrgToStart = false;
    const char* prgName;

    for ( size_t i = 0; i < prgManager_prgList_length; i++ ) {
        prgName = prgManager_list [ i ];
        printf( "init prgName: %s \n", prgName );
        if ( prgName == NULL ) continue;
        if ( foundPrgToStart == false && strcmp( startPrgName, prgName ) == 0 ) {
            foundPrgToStart = true;
        }

        if ( cleanup != NULL ) {
            printf( "prgNameCleanup: %s \n", prgName );
            folder_remove( path_build_get( prgName ) );
        }

        if ( !prgManager_timestamp_compile_changed( prgName ) &&
             file_exist( path_build_get( prgName ) ) ) {
            printf( "No changes on prg: %s\n", prgName );
            continue;
        }

        printf( "prgNameCompile: %s \n", prgName );
        prgManager_compile( prgName, true );
    }
    if ( foundPrgToStart && !prgRunning ) {
        prgRunning = true;
        prgManager_watch( startPrgName );
    } else {
        printf( "%s not found or PRG running. prgRunning: %i\n", startPrgName, prgRunning );
    }
    return 0;
}
