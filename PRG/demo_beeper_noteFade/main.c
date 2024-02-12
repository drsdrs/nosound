
float bpm          = 160;
float ticksPerBeat = 24;

int beats;
int ticks;

void setup() {
    fps_set( ( bpm / 60 ) * ticksPerBeat );
    scale_set( 1 );
    Beeper_set( 0, 0, 0 );
}

void loop() {
    // tv_clear( COLORS[( 16 - beats ) & 0xf] );
    static float stepSize;
    static int   stepDelay;
    static int   noteBegin;
    static int   noteEnd;
    if ( ticks < ticksPerBeat ) {
        ticks++;
        // if(ticks==12)Beeper_set( 0, 127, 7 );
        // if(ticks==13)Beeper_set( 0, 0, rand()&7 );
    } else {
        //	uint8_t channel=0, uint8_t note_start=64, uint8_t note_end=88, float step_inc=.1, uint16_t step_delay_us=4, uint16_t length_us=500

        noteBegin       = 66;    // rand()&127;
        noteEnd         = 88;    // 63+(rand() & 63);
        int stepSizeRnd = ( rand() %127 ) + 1;
        stepSize        = 1 / (float)( stepSizeRnd );
        stepDelay       = ( rand() & 127 );
        Beeper_note_fade( 0, noteBegin, noteEnd, stepSize, stepDelay + 1, 500 );
        beats++;
        ticks = 0;
    }

    border( 0, 0, ROWS, 2, 0, COLORS[4] );

    print_center( WIDTH / 3, 8 * 5, 0xfff, "stepSize: %4.4f", stepSize );
    print_center( WIDTH / 3, 8 * 6, 0xeee, "stepDelay: %4i", stepDelay );
    print_center( WIDTH / 3, 8 * 7, 0xf00, "noteBegin: %4i", noteBegin );
    print_center( WIDTH / 3, 8 * 8, 0xf0f, "noteEnd: %4i", noteEnd );
    print_center( WIDTH / 2, 24, COLORS[beats % 0xf], "%2.i, %3.i", ticks, beats );
    border( 0, 16, ROWS / 3 * 2 + 1, LINES - 2, 0, COLORS[2] );
    border( ( 1 + ROWS / 3 * 2 ) * 8, 16, ROWS / 3, LINES - 2, 0, COLORS[10] );
}
