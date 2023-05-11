#include "./tv.h"
#include <stdlib.h>

void main(){

	setup();

	for(;;){
			while( loop() ){ };
			break;
	}
	tv_exit();
	exit(1);
}
