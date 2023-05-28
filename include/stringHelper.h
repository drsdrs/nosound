#pragma once

extern char*  string_temp_get( char* fmt, ... );
extern char*  string_append( const char* str0, const char* str1 );
extern char** string_split( char* str, const char* delimiters );
extern char*  string_join( char** str, const char* delimiters );
