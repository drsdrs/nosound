#include "path.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "stringHelper.h"

char* path_cwd_get() {
    static char cwdPath [ 0xff ];
    if ( getcwd( cwdPath, sizeof( cwdPath ) ) != NULL ) { // TODO make path only once
        int bytes = fmin( readlink( "/proc/self/exe", cwdPath, 0xff ), 0xff - 1 );
        if ( bytes >= 0 ) cwdPath [ bytes ] = '\0';
    }
    return cwdPath;
}

char* path_root_get() {    // get abs path from nosound dir
    static char* pathRes = "\0";
    if ( strlen( pathRes ) > 8 ) return pathRes;    // only make root path once!
    pathRes            = string_append( pathRes, "/" );
    char*  cwdPath     = path_cwd_get();
    char** cwdSplit    = string_split( cwdPath, "/" );
    size_t cwdSplitLen = sizeof( cwdSplit );
    char*  rootDir     = "nosound";
    for ( size_t i = 0; i < cwdSplitLen; i++ ) {
        // printf("pathres: %s\n", pathRes);
        pathRes = string_append( pathRes, cwdSplit [ i ] );
        pathRes = string_append( pathRes, "/" );
        if ( strcmp( rootDir, cwdSplit [ i ] ) == 0 ) {
            return pathRes;
        }
    }
    printf( "Get root path error, NULL\n" );
    return NULL;
}

char* path_binary_get( const char* prgName ) {
    return string_temp_get( "PRG/%s/main", prgName, prgName );
}

char* path_savefile_get( const char* prgName ) {
    return string_temp_get( "PRG/%s/assets/savefile", prgName );
}

char* path_build_get( const char* prgName ) {
    return string_temp_get( "%sPRG/%s/build", path_root_get(), prgName );
    // return string_temp_get( "PRG/%s/build", prgName);
}

char* path_source_get( const char* prgName ) {
    return string_temp_get( "PRG/%s/main.c", prgName );
}

char* path_cmakelist_get( const char* prgName ) {
    return string_temp_get( "PRG/%s/build/CMakeLists.txt", prgName );
}

char* path_wrapper_get( const char* prgName ) {
    return string_temp_get( "PRG/%s/build/prg_wrapper.c", prgName );
}

char* path_timestamp_get( const char* prgName ) {
    return string_temp_get( "PRG/%s/build/timestamp_compile", prgName );
}

char* path_spritesheet_get( const char* prgName ) {
    return string_temp_get( "PRG/%s/assets/spritesheet.png", prgName );
}
char* path_spritesheetAtlas_get( const char* prgName ) {
    return string_temp_get( "PRG/%s/assets/spritesheet.json", prgName );
}
char* path_assets_get( const char* prgName ) {
    return string_temp_get( "PRG/%s/assets", prgName );
}
