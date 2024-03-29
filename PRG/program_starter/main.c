#include "prgManager.h"

int8_t   scale = 1;
uint16_t cnt;
char*    nextPrg;
uint16_t k, r;
uint8_t  cursorY;

uint16_t    prgListLength;
char**      prgList;
const char* myPrgName;

void setup() {
    fps_set( 60 );
    prgManager_list_get( &prgList, &prgListLength );
    myPrgName = prg_name_get();
}

void loop() {
    for ( size_t prgIndex = 0; prgIndex < prgListLength; prgIndex++ ) {
        if ( cursorY == prgIndex ) lineY( 13 + 8 * prgIndex, 0x444 );
        print_center( WIDTH / 2, 12 + 8 * prgIndex, 0xf0f, "%s", prgList [ prgIndex ] );
    }

    print_center( WIDTH / 2, HEIGHT / 8 * 5, 0x0ff, "loop_frames =   %12i", loop_frames );
    print_center( WIDTH / 2, HEIGHT / 8 * 6, 0x0ff, "loop_interval_us = %12ims", loop_interval_us );
    print_center( WIDTH / 2, HEIGHT / 8 * 7, 0x0ff, "FPS =   %2.2f", fps_get() );
    print_center( WIDTH / 2, HEIGHT / 2, 0xfff, "prg_name_get() = %s", myPrgName );

    if ( key_pressed( BTN_UP ) ) {
        cursorY--;
        cursorY %= prgListLength;
        //Beeper_note_fade( 0, 64, 100, 0.02, 170, 750 );
        Beeper_set(0, 80, 10);
        loop_sleep_ms(125);
        Beeper_set(0, 1, 0);
    } else if ( key_pressed( BTN_DOWN ) ) {
        cursorY++;
        //Beeper_note_fade( 0, 100, 64, 0.01, 270, 750 );
        Beeper_set(0, 90, 10);
        loop_sleep_ms(125);
		Beeper_set(0, 1, 0);
        cursorY %= prgListLength;
    } else if ( key_pressed( BTN_A ) ) {
		//Beeper_note_fade( 0, 0, 127, 0.1, 27, 750 );
        Beeper_set(0, 99, 10);
        loop_sleep(1);
        loop_exit();
        // sleep(5);
        prg_change( prgList [ cursorY ] );
    }

}
