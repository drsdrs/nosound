#include <math.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "path.h"

#include "prg.h"

char path[0xff];

const char* prg_name_get(){
  char* path = path_cwd_get();
  char *token = strtok(path, "/");
  while(token) {
    //printf("TOKEN: %s\n", token);
    if( strcmp( token, "PRG" )==0 ){
      token = strtok(NULL, "/");
      return token;
    }
    token = strtok(NULL, "/");
  }

  return NULL;
}

void prg_change( char* prgName ){
  //loop_exit(); // have to be in scope !!!
  //usleep(1000); // alsa need ~>500ms to cool down ELSE  bad descriptor error
  char* argv[] = { path_binary_get( prgName ), NULL };
  printf("PRG_CHANGE: %s\n", path_binary_get( prgName ) );
  int child_status = execvp(argv[0], argv);
  printf("ERROR - prg_change -?? Unknown command status: %i\t%s\n", child_status, argv[0]);/* If execvp returns, it must have failed. */
  exit(0);
}


void prg_data_save( uint16_t pos, uint8_t val ){
  FILE *f;
  char buf[1] = { val };

  f = fopen( path_savefile_get(prg_name_get()), "w");          // Opening in write mode

  if( f == NULL ) {
    printf("Error data_local_save\ttrg: %s\n", path_savefile_get(prg_name_get()) );
    exit(EXIT_FAILURE);
  }

  fseek(f, pos, SEEK_SET);
  fwrite(buf, 1, 1, f);

  fclose(f);
}


uint8_t prg_data_load( uint16_t pos ){
  FILE *f;
  char buf[1];

  f = fopen( path_savefile_get(prg_name_get()) , "r");   //opening the file for reading.

  if( f == NULL ) {
    printf("Error data_local_load\ttrg: %s\n", path_savefile_get(prg_name_get()) );
    exit(EXIT_FAILURE);
  }

  memset(buf, 0, 1);
  fseek(f, pos, SEEK_SET);
  fread(buf, 1, 1, f);

  fclose(f);
  return buf[1];
}
