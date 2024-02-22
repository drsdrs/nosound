# nosound
Little C programming framework.
(WIP - work in progress)


#### Run in dev mode.
'./buildRun.sh arg0'

arg0 = start program

use "prgManagerUI" to start program selection screen


Source file "main.c" in the PRG folder are watched and compiled on change.


## How to use

### main.c
PRG/*/main.c must contains:

1: 'void setup(){}'

2: 'void loop(){}'

### loop function timeout
The loop function gets executed in constant time intervals.

The 'setTimeout( interval )' function sets the loop time in nanoseconds.

###	Inputs

Input keys are polled in the time between loops.

Use 'tv_key_pressed( keyToCheck )


### TODO
TODO desc. of header files to use for audio/video
