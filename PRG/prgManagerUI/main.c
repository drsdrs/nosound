int8_t scale = 1;
uint16_t cnt;
char* nextPrg;
uint16_t k,r;
uint8_t cursorY;

uint16_t prgListLength;
char** prgList;
char* myPrgName;


void setup(){
  loop_interval_set(50);
  prgManager_list_get( &prgList, &prgListLength );
  myPrgName = prg_name_get();
}


void loop(){
  tv_clear(0);
  for (size_t prgIndex = 0; prgIndex < prgListLength; prgIndex++) {
    if( cursorY==prgIndex )
    tv_lineY( 13+8*prgIndex, 0x444 );
    tv_print( WIDTH/2, 12+8*prgIndex, 0xf0f, "%s", prgList[ prgIndex ] );
  }

  tv_print( WIDTH/2, HEIGHT/8*5, 0x0ff, "loop_frames:   %12i", loop_frames );
  tv_print( WIDTH/2, HEIGHT/8*6, 0x0ff, "loop_interval_us: %12ims", loop_interval_us );
  tv_print( WIDTH/2, HEIGHT/8*7, 0x0ff, "loop_millis:   %12ims", loop_millis );
  tv_print( WIDTH/2, HEIGHT/2, 0xfff, "Prg.name: %s", myPrgName);

  if( tv_key_pressed( BTN_UP ) ){
    cursorY--;
    cursorY %= prgListLength;
  } else if( tv_key_pressed( BTN_DOWN ) ){
    cursorY++;
    cursorY %= prgListLength;
  } else if( tv_key_pressed( BTN_A ) ){
    loop_exit(); // have to be in scope !!!
    //sleep(5);
    prg_change( prgList[ cursorY ] );
  }
  //SDL_Delay(25);

  tv_render();

}
