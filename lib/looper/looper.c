#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "looper.h"

#include "../beeper/beeper.h"
#include "../prgManager/prgManager.h"
#include "../tv/tv.h"
#include "../waves/waves.h"


struct timespec start, end, finish;
uint32_t loop_interval_ns = 250;

int64_t delta_ns;
int64_t diff_ns;
int64_t diff_us_averaged;
uint64_t slept_ns;
uint64_t loop_frames;
uint64_t loop_lifetime_ns;

uint8_t loop_quit;

void loop_sleep_ns( uint32_t sleep_ns ){ // in-accurate // TODO
  static struct timespec start, end;
  uint32_t slept_ns_temp;
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  while( slept_ns_temp < sleep_ns ){
    tv_key_poll( );
    SDL_Delay(0);
    clock_gettime(CLOCK_MONOTONIC_RAW, &finish);
    slept_ns_temp = (finish.tv_sec - start.tv_sec) * 1000000 + (finish.tv_nsec - start.tv_nsec) / 1000;
  }

  slept_ns += slept_ns_temp;
}


void loop_sleep( uint32_t sleep_us ){ // in-accurate // TODO
  loop_sleep_ns(sleep_us*1000);
}

void loop_loop ( void (*loop_funct)() ) {
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);

  slept_ns = 0;

  tv_clear(0);
  loop_funct();
  tv_render();

  tv_keys_clear();
  tv_key_poll( );
  loop_frames++;

  clock_gettime(CLOCK_MONOTONIC_RAW, &end);

  delta_ns = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
  diff_ns = (loop_interval_ns) - (delta_ns);
  diff_ns += slept_ns;

  if (diff_ns < 0 && loop_interval_ns > 0) { //
      printf("Loop needs too much time, overrun us:%li\n", diff_ns );
      loop_lifetime_ns += delta_ns;
      diff_us_averaged = (diff_us_averaged + diff_ns)>>1;
      return;
  }

  loop_sleep_ns(diff_ns);

  loop_lifetime_ns += delta_ns+slept_ns;
  diff_us_averaged = (diff_us_averaged + diff_ns)>>1;
  //printf("Loop diff_us_averaged:%li\n", diff_us_averaged );
}

void loop_exit(){
  printf("void loop_exit()\n\n");
  loop_quit = true;
  Beeper_exit();
  tv_exit();
}


void loop_interval_set( uint32_t new_interval ){
  loop_interval_ns = new_interval;
}


long int loop_measure_ns( uint8_t start_end ){
  static struct timespec start, end;
  static long int avg = -1;
  long int res;

  if( start_end==MEASURE_START ){
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    res = 0;
    //avg = -1;
  } else if( start_end==MEASURE_STOP || start_end==MEASURE_STOP_AVG ){
    start = end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    res = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
  }

  if( start_end==MEASURE_STOP_AVG ){
    if(avg==-1){
      avg = res;
    } else {
      res = avg*.98f + res*.02f;
      avg = res;
    }
  }

  return res;

}

void loop_setup_PRIVATE( void (*setup_funct)(), void (*loop_funct)(), uint32_t new_interval ){
  printf("Start loop_setup_PRIVATE\n");
  loop_frames = 0;
  loop_interval_ns = new_interval;

  loop_measure_ns( MEASURE_START );

  tv_setup();
  printf("tv_setup took: %li ns\n", loop_measure_ns( MEASURE_STOP ));

  Beeper_setup();
  printf("Beeper_setup took: %li ns\n",  loop_measure_ns( MEASURE_STOP ));

  waves_all_generate();
  printf("waves_all_generate took: %li ns\n",  loop_measure_ns( MEASURE_STOP ));

  setup_funct();
  printf("setup_funct took: %li ns\n",  loop_measure_ns( MEASURE_STOP ));

  tv_clear(0);
  while( loop_quit==false ) loop_loop( loop_funct );

  printf("Final PRG exit in loop_setup_PRIVATE\n");
  exit(0);
}
