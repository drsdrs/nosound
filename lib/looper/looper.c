#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "looper.h"

#include "../beeper/beeper.h"
#include "../prgManager/prgManager.h"
#include "../tv/tv.h"
#include "../waves/waves.h"


struct timespec start, end, finish;
uint32_t loop_interval_us = 250;

int64_t delta_us;
int64_t diff_us;
int64_t diff_us_averaged;
uint64_t slept_us;
uint64_t loop_frames;
uint64_t loop_millis;

uint8_t loop_quit;


void loop_sleep( uint32_t sleep_us ){
  SDL_Delay( sleep_us );
  slept_us += sleep_us;
}

void loop_loop ( void (*loop_funct)() ) {
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);

  slept_us = 0;
  loop_funct();
  loop_frames++;

  tv_keys_clear();

  clock_gettime(CLOCK_MONOTONIC_RAW, &end);

  delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
  diff_us = (loop_interval_us) - (delta_us);
  diff_us += slept_us;

  if (diff_us < 0 && loop_interval_us > 0) { //
      printf("Loop needs too much time, overrun us:%li\n", diff_us );
      return;
  }

  tv_key_poll( );
  while( diff_us >= -diff_us_averaged && loop_interval_us!=-1){    //     finish needed time to poll keys
      //for (size_t i = 0; i < 0xf; i++) {
        tv_key_poll( );
        SDL_Delay(0);
      //}

      clock_gettime(CLOCK_MONOTONIC_RAW, &finish);
      delta_us = (finish.tv_sec - start.tv_sec) * 1000000 + (finish.tv_nsec - start.tv_nsec) / 1000;
      diff_us = (loop_interval_us) - (delta_us);
      diff_us += slept_us;
  }
  loop_millis += delta_us;
  diff_us_averaged = (diff_us_averaged + diff_us)>>1;
  //printf("Loop diff_us_averaged:%li\n", diff_us_averaged );
}

void loop_exit(){
  printf("void loop_exit()\n\n");
  loop_quit = true;
  Beeper_exit();
  tv_exit();
  exit(1);
}


void loop_interval_set( uint32_t new_interval ){
  loop_interval_us = new_interval;
}

void loop_setup_PRIVATE( void (*setup_funct)(), void (*loop_funct)(), uint32_t new_interval ){
  printf("Start loop_setup_PRIVATE\n");
  loop_frames = 0;
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC_RAW, &end);

  tv_setup();
  start = end;
  clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  delta_us = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;

  printf("SDL_SETUP took: %li ms\n", delta_us);

  Beeper_setup();
  start = end;
  clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  delta_us = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
  printf("Beeper_setup took: %lims\n", delta_us);

  loop_interval_us = new_interval;

  waves_all_generate();
  // start = end;
  // clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  // delta_us = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
  // printf("waves_all_generate took: %lims\n", delta_us);

  setup_funct();

  while( loop_quit==false ) loop_loop( loop_funct );

}
