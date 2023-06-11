#pragma once
#include <stdint.h>

extern int     folder_remove( char* path );
extern void    folder_create( char* dirName );
extern uint8_t file_exist( char* fileName );
extern void    file_write( char* fileName, char* content );
extern void    file_copy( char* src, char* trg );
extern char*   file_read( char* fileName );
