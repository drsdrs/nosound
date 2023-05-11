#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>


char* testStrings[3] = {
  "asd",
  "1231",
  "i8i8i"
};

char** trgArr;
int arrLen;


void assignArr( char*** trg ){
  char** tmpArr = malloc(sizeof(tmpArr));
  for (size_t i = 0; i < sizeof(testStrings); i++) {
    tmpArr = (char**) realloc(tmpArr, sizeof(char*) * (i+1));
    if(tmpArr==NULL){
      printf("MEMORY ERROR");
      exit(1);
    }

    tmpArr[i] = (char*) malloc( sizeof( testStrings[i] ) );
    strcpy(tmpArr[i], testStrings[i]);
    //tmpArr[i] = testStrings[i];
  }
  //printf("pointer trg tmArr: %d %d \n", trg, &tmpArr);
  *trg = tmpArr;
  //printf("pointer trg tmArr: %d %d \n", trg, &tmpArr);
}

void main(){
  printf("sizeof(testStrings): %u\n", sizeof(testStrings)/sizeof(testStrings[0]));
  printf("trgArr pointer: %u\n", &trgArr);
  assignArr( &trgArr );
  printf("trgArr pointer: %u\n", &trgArr);
  printf("trgArr[0] pointer: %u\n", &trgArr[0]);
  printf("trgArr[1] pointer: %u\n", &trgArr[1]);
  printf("trgArr[2] pointer: %u\n", &trgArr[2]);
  printf("trgArr[2][0] pointer: %u\n", &trgArr[2][0]);
  printf("trgArr[0]: %s\n", trgArr[0]);
  printf("trgArr[1]: %s\n", trgArr[1]);
  printf("trgArr[2]: %s\n", trgArr[2]);
  printf("trgArr[3]: %s\n", trgArr[3]);
}
