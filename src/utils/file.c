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

int folder_remove( char* path ) {
    DIR*   d        = opendir( path );
    size_t path_len = strlen( path );
    int    r        = -1;
    if ( d ) {
        struct dirent* p;
        r = 0;
        while ( !r && ( p = readdir( d ) ) ) {
            int    r2 = -1;
            char*  buf;
            size_t len;
            /* Skip the names "." and ".." as we don't want to recurse on them. */
            if ( !strcmp( p->d_name, "." ) || !strcmp( p->d_name, ".." ) ) continue;
            len = path_len + strlen( p->d_name ) + 2;
            buf = malloc( len );
            if ( buf ) {
                struct stat statbuf;
                snprintf( buf, len, "%s/%s", path, p->d_name );
                if ( !stat( buf, &statbuf ) ) {
                    if ( S_ISDIR( statbuf.st_mode ) ) {
                        r2 = folder_remove( buf );
                    } else {
                        r2 = unlink( buf );
                    }
                }
                free( buf );
            }
            r = r2;
        }
        closedir( d );
    }
    if ( !r ) r = rmdir( path );
    return r;
}

void folder_create( char* dirName ) {
    struct stat st = { 0 };
    if ( stat( dirName, &st ) == -1 ) {
        mkdir( dirName, 0700 );
    }
}

void file_copy( char* src, char* trg ) {
    char  c[4096];    // or any other constant you like
    FILE* stream_R = fopen( src, "r" );
    FILE* stream_W = fopen( trg, "w" );    // create and write to file

    while ( !feof( stream_R ) ) {
        size_t bytes = fread( c, 1, sizeof( c ), stream_R );
        if ( bytes ) {
            fwrite( c, 1, bytes, stream_W );
        }
    }
    fclose( stream_R );
    fclose( stream_W );
}

uint8_t file_exist( char* fileName ) {
    struct stat st = { 0 };
    if ( stat( fileName, &st ) == -1 ) {
        return false;
    }
    return true;
}

void file_write( char* fileName, char* content ) {
    FILE* f = fopen( fileName, "w" );
    if ( f == NULL ) {
        printf( "Error opening file for write: %s\n", fileName );
        exit( 1 );
    }
    fprintf( f, "%s", content );
    fclose( f );
    printf( "Done writing %s!\n", fileName );
}

char* file_read( char* fileName ) {
    FILE* fp;
    long  lSize;
    char* buffer;

    fp = fopen( fileName, "rb" );
    if ( !fp ) perror( fileName ), exit( 1 );

    fseek( fp, 0L, SEEK_END );
    lSize = ftell( fp );
    rewind( fp );

    /* allocate memory for entire content */
    buffer = calloc( 1, lSize + 1 );
    if ( !buffer ) fclose( fp ), fputs( "memory alloc fails", stderr ), exit( 1 );
    /* copy the file into the buffer */
    if ( 1 != fread( buffer, lSize, 1, fp ) ) fclose( fp ), free( buffer ), fputs( "entire read fails", stderr ), exit( 1 );

    fclose( fp );
    return buffer;
}
