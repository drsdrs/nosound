#pragma once

extern char** prgManager_list;
extern void prgManager_watch_exit();
extern int prgManager_init( const char* startPrgName, const char* cleanup );
extern void prgManager_list_get( char*** fileListPointer, uint16_t* fileListLengthPointer );
extern uint8_t prgManage_run( const char* prgName );
extern void prgManager_watch( const char* prgName );
