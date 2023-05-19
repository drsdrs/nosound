#include "../../lib/entity/entity.c"
#include <sys/resource.h>



uint64_t deltaAvg;
uint64_t startMem;
int i=0;


uint64_t prg_memory_usage(){
	static struct rusage r_usage;

	getrusage(RUSAGE_SELF,&r_usage);
	return r_usage.ru_maxrss;
}

void setup(){
	tv_clear(1);
	tv_scale_set(2);
	//loop_interval_set(1000000/60);
	loop_interval_set(1000);
	startMem = prg_memory_usage();
}


void loop(){
	//testEntity(i++);
	deltaAvg = deltaAvg*0.99f + loop_delta_full_ns*0.01f;

	tv_print( WIDTH/2, HEIGHT/8*5, 0xfff, "delta: %7i us", deltaAvg/1000 );
	// tv_print( WIDTH/2, HEIGHT/8*7, 0xfff, "fps: %f ", 1/(deltaAvg/1000000000.0f) );

  	tv_print( WIDTH/2, HEIGHT/8*6, 0xfff, "Mem: %ld kb\n", prg_memory_usage()-startMem);

  	struct timespec time;
	clock_gettime(CLOCK_MONOTONIC_RAW, &time);
  	double number_of_processors = sysconf(_SC_NPROCESSORS_ONLN);
  	tv_print(
  		WIDTH/2, HEIGHT/8*7,
  		0xfff,
  		"CPU: %f %\n", 
  		((double)clock()/number_of_processors/(time.tv_sec+time.tv_nsec/1000000000))
  	);
 
  if(prg_memory_usage()>1300000) loop_exit();

  for (int i = 1; i < 0xff; ++i) {
  	for (int u = 1; u < 0xff; ++u){
	  	//char* doing = malloc(1);
  		int a = 99999/u*i/99999*sin(324234)*sin(234234);
  	}
  }
  // Print the maximum resident set size used (in kilobytes).
}
