
uint16_t sinPos;
uint16_t sinPosMain;


char* textSine = "!!! Cracked by DRS !!!";
char* textTicker = "Intently sniff hand thinking about you i'm joking it's food always food. Human is in bath tub, emergency! drowning! meooowww!. Furrier and even more furrier hairball sleeping in the box yet naughty running cat for blow up sofa in 3 seconds, or get away from me stupid dog meow to be let in one of these days i'm going to get that red dot, just you wait and see . Run around the house at 4 in the morning see brother cat receive pets, attack out of jealousy growl at dogs in my sleep claw drapes, and play riveting piece on synthesizer keyboard so i see a bird i stare at it i meow at it i do a wiggle come here birdy. Reaches under door into adjacent room demand to be let outside at once, and expect owner to wait for me as i think about it that box? i can fit in that box, and go crazy with excitement when plates are clanked together signalling the arrival of cat food. Scratch so owner bleeds cry louder at reflection. Push your water glass on the floor run in circles eat prawns daintily with a claw then lick paws clean wash";
int text_len;
int textSineX;
int textSineY;
float modX;
float charY;

int circX=0;
int circY=0;

int tickerX=0;
int tickerLength;
int tickerY=0;


void setup(){

  tv_scale_set(1);

  text_len = strlen(textSine);
  tickerLength = strlen(textTicker)*8;
  tickerX = WIDTH;
  loop_interval_set(1000000/30);
  textSineX = ( WIDTH - strlen(textSine)*8 ) / 2 ;
  textSineY = HEIGHT/2;
}

int note=88;

void loop(){ // need phase inc based on inverval // OR fixed delta based on sec=1000000us
  loop_measure_ns( MEASURE_START );

  if( (rand()&0x7) == 0 ) Beeper_set(0, note, 5);
  loop_sleep(16);
  Beeper_set(0, 0, 0);

  tv_clear(0);
  circX += 16;
  circY += 16;
  circX &= 0xfff;
  circY &= 0xfff;
  int x = (WIDTH2)+(WIDTH2-CHAR_HEIGHT) * waves_sine12_get(circX);
  int y = (HEIGHT2)+(HEIGHT2-CHAR_HEIGHT) * waves_sine12_get((0x1000/4+circY)&0xfff);


  //printf("%i _ %i \t\t\t\t\n",u, i);
  tv_print( x, y, COLORS[(int)(HEIGHT-y)/31], "\1" );

  tv_print( tickerX, HEIGHT-CHAR_HEIGHT, 0x0ff, textTicker );
  tickerX-=2;
  if(tickerX<-tickerLength/2) tickerX=WIDTH;

  for (size_t charX = 0; charX < text_len; charX++) {
    char ch = textSine[charX];
    modX = waves_sine12_get(sinPos+charX*24);
    charY = (waves_sine12_get(sinPos+charX*66)) * (waves_sine12_get(sinPosMain+charX*77));
    tv_char( WIDTH2/2+WIDTH2/2*waves_sine12_get(circX)+charX*8, (HEIGHT2-CHAR_HEIGHT*4)+(HEIGHT2-CHAR_HEIGHT*4 )*charY, ch, COLORS[(int)((1+charY)*8)] );
    sinPos += 2+charY;
    sinPosMain += 1+modX;
  }

  tv_print( HEIGHT/8*7, WIDTH/2, 0xfff, "Loop needed %4i ns", loop_measure_ns( MEASURE_STOP_AVG ) );
  tv_render();
}
