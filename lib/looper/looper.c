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


void loop_sleep( uint32_t sleep_us ){ // in-accurate // TODO
  uint32_t sleep_us_temp = sleep_us;
  while( sleep_us_temp-- ){
    tv_key_poll( );
    SDL_Delay( 1 );
  }


  slept_us += sleep_us;
}

void loop_loop ( void (*loop_funct)() ) {
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);

  slept_us = 0;
  loop_funct();

  tv_keys_clear();

  tv_key_poll( );

  loop_frames++;


  clock_gettime(CLOCK_MONOTONIC_RAW, &end);

  delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
  diff_us = (loop_interval_us) - (delta_us);
  diff_us += slept_us;

  if (diff_us < 0 && loop_interval_us > 0) { //
      printf("Loop needs too much time, overrun us:%li\n", diff_us );
      return;
  }

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
}


void loop_interval_set( uint32_t new_interval ){
  loop_interval_us = new_interval;
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
  loop_interval_us = new_interval;

  loop_measure_ns( MEASURE_START );

  tv_setup();
  printf("tv_setup took: %li ns\n", loop_measure_ns( MEASURE_STOP ));

  Beeper_setup();
  printf("Beeper_setup took: %li ns\n",  loop_measure_ns( MEASURE_STOP ));

  waves_all_generate();
  printf("waves_all_generate took: %li ns\n",  loop_measure_ns( MEASURE_STOP ));

  setup_funct();
  printf("setup_funct took: %li ns\n",  loop_measure_ns( MEASURE_STOP ));

  while( loop_quit==false ) loop_loop( loop_funct );

  printf("Final PRG exit in loop_setup_PRIVATE\n");
  exit(0);
}
