int8_t scale = 0;
uint8_t defaultScale = 2;
uint16_t k;

void setup(){
  loop_interval_set(250);
}



uint8_t test_scale(){
  if(scale==6){
    tv_scale_set( defaultScale );
    return false;
  }
  tv_scale_set(scale);
  tv_clear(0);

  int col = 0;
  for (size_t y = 0; y < HEIGHT; y++) { for (size_t x = 0; x < WIDTH; x++) {
    col += y;
    col &= 7;
    int ccol = (col+(col<<4)+(col<<8));
    tv_pixel(x, y, ccol);
  }}


  if( scale>3 ){
    tv_print( (WIDTH/2), 3, 0xfff, "W%i", WIDTH);
    tv_print( (WIDTH/2), (HEIGHT/2)+4, 0xfff, "H%i", HEIGHT);
  } else {
    tv_print( (WIDTH/2), (HEIGHT/2)*.5, 0xfff, "Scale: %i", scale);
    tv_print( (WIDTH/2), (HEIGHT/2), 0xfff, "HEIGHT: %i", HEIGHT);
    tv_print( (WIDTH/2), (HEIGHT/2)*1.5, 0xfff, "WIDTH: %i", WIDTH);

  }

  if( k=tv_key_pressed( 0 ) ){
    printf("Pushed!!!\t\t");
    switch(k){
      case BTN_UP: printf("BTN_UP\n"); break;
      case BTN_RIGHT: printf("BTN_RIGHT\n"); break;
      case BTN_LEFT: printf("BTN_LEFT\n"); break;
      case BTN_DOWN: printf("BTN_DOWN\n"); break;
      case BTN_A: printf("BTN_A\n"); break;
      case BTN_B: printf("BTN_B\n"); break;
      case BTN_X: printf("BTN_X\n"); break;
      case BTN_Y: printf("BTN_Y\n"); break;
      case BTN_START: printf("BTN_START\n"); break;
      case BTN_SELECT: printf("BTN_SELECT\n"); break;
      case BTN_S_RIGHT: printf("BTN_S_RIGHT\n"); break;
      case BTN_S_LEFT: printf("BTN_S_LEFT\n"); break;
    }
  }

  tv_render();
  scale++;
  return true;
}

void showTestName( char* testName ){
  tv_clear(0);
  tv_print( WIDTH/2, HEIGHT/2, 0xfff, "%s", testName);
  tv_render();
  loop_sleep(1000);
}

void loop(){
  scale = 0;
  showTestName( "Scaling Test" );
  while( test_scale() ){ loop_sleep(1000); }
  showTestName( "Scaling Test - END" );

}
