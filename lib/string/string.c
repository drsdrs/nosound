//#include "assert.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>


char* string_temp_get(char *fmt, ...) {
  static char buf[99];
  va_list va;
  va_start (va, fmt);
  vsprintf (buf, fmt, va);
  va_end (va);
  //printf("tempString: %s\n", buf);
  return buf;
}

char* string_append( char* str0, char* str1 ){
  char * new_str ;
  if((new_str = malloc( strlen(str0)+strlen(str1)+1) ) != NULL){
      new_str[0] = '\0';   // ensures the memory is an empty string
      strcat(new_str, str0);
      strcat(new_str, str1);
  } else {
      printf("malloc failed!\n");
  }
  return new_str;
}

char** string_split(char *str, const char *delimiters){
    char * token;
    char **tokenArray;
    int count=0;
    token = (char *)strtok(str, delimiters); // Get the first token
    tokenArray = (char**)malloc(1 * sizeof(char*));
    tokenArray[0] = NULL;
    if (!token) {
        return tokenArray;
    }
    while (token != NULL) { // While valid tokens are returned
        tokenArray[count] = (char*)strdup(token);
        //printf ("%s", tokenArray[count]);
        count++;
        tokenArray = (char **)realloc(tokenArray, sizeof(char *) * (count + 1));
        token = (char *)strtok(NULL, delimiters); // Get the next token
    }
    tokenArray[count] = NULL;  /* Terminate the array */
    return tokenArray;
}

char* string_join(char **str, const char *delimiters){
    char *joinedStr;
    int i = 1;
    joinedStr = realloc(NULL, strlen(str[0])+1);
    strcpy(joinedStr, str[0]);
    if (str[0] == NULL){
        return joinedStr;
    }
    while (str[i] !=NULL){
        joinedStr = (char*)realloc(joinedStr, strlen(joinedStr) + strlen(str[i]) + strlen(delimiters) + 1);
        strcat(joinedStr, delimiters);
        strcat(joinedStr, str[i]);
        i++;
    }
    return joinedStr;
}
//
// char** string_split(char* a_str, const char a_delim){
//     char** result    = 0;
//     size_t count     = 0;
//     char* tmp        = a_str;
//     char* last_comma = 0;
//     char delim[2];
//     delim[0] = a_delim;
//     delim[1] = 0;
//     while (*tmp){/* Count how many elements will be extracted. */
//         if (a_delim == *tmp){
//             count++;
//             last_comma = tmp;
//         }
//         tmp++;
//     }
//     /* Add space for trailing token. */
//     count += last_comma < (a_str + strlen(a_str) - 1);
//     /* Add space for terminating null string so caller knows where the list of returned strings ends. */
//     count++;
//     result = malloc(sizeof(char*) * count);
//
//     if (result){
//         size_t idx  = 0;
//         char* token = strtok(a_str, delim);
//
//         while (token) {
//             assert(idx < count);
//             *(result + idx++) = strdup(token);
//             token = strtok(0, delim);
//         }
//         assert(idx == count - 1);
//         *(result + idx) = 0;
//     }
//
//     return result;
// }
