#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

#include <time.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "file.h"


int folder_remove(char *path) {
   DIR *d = opendir(path);
   size_t path_len = strlen(path);
   int r = -1;
   if (d) {
      struct dirent *p;
      r = 0;
      while (!r && (p=readdir(d))) {
          int r2 = -1;
          char *buf;
          size_t len;
          /* Skip the names "." and ".." as we don't want to recurse on them. */
          if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) continue;
          len = path_len + strlen(p->d_name) + 2;
          buf = malloc(len);
          if (buf) {
             struct stat statbuf;
             snprintf(buf, len, "%s/%s", path, p->d_name);
             if (!stat(buf, &statbuf)) {
                if (S_ISDIR(statbuf.st_mode)){
                  r2 = folder_remove(buf);
                } else {
                  r2 = unlink(buf);
                }
             }
             free(buf);
          }
          r = r2;
      }
      closedir(d);
   }
   if (!r) r = rmdir(path);
   return r;
}

void folder_create(char* dirName){
  struct stat st = {0};
  if (stat(dirName, &st) == -1) {
    mkdir(dirName, 0700);
  }
}


void file_copy( char* src, char* trg ){
  FILE *f1, *f2;
  char ch;

  f1 = fopen(src, "r");   //opening the file for reading.
  f2 = fopen(trg, "w");          // Opening in write mode

  if(f2 == NULL || f1 == NULL) {
    printf("Error file_copy src: %s\t\ttrg: %s\n", src, trg);
    exit(EXIT_FAILURE);
  }

  while((ch = fgetc(f1)) != EOF) fputc(ch,f2);
  fclose(f1);
  fclose(f2);
}

uint8_t file_exist(char* fileName){
  struct stat st = {0};
  if (stat(fileName, &st) == -1) {
      return false;
  }
  return true;
}

void file_write( char* fileName, char* content ){
  FILE *f = fopen(fileName, "w");
  if (f == NULL){
      printf("Error opening file for write: %s\n", fileName);
      exit(1);
  }
  fprintf(f, "%s", content);
  fclose(f);
  printf("Done writing %s!\n", fileName);
}
