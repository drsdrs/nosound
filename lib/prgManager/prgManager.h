#pragma once


#include "../path/path.h"
#include "../file/file.h"
#include "../string/string.h"
#include "../term/term.h"

extern char** prgManager_list;
extern void prgManager_watch_exit();
extern void prgManager_init( const char* prgName, const char* cleanup );
extern char* prgManager_list_get(char*** fileListPointer, uint16_t* fileListLengthPointer );
extern uint8_t prgManage_run( const char* prgName );
extern void prgManager_watch( const char* prgName );

extern char* path_root_get();
