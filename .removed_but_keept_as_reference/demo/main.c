
double sinPos;
double sinPosMain;

char* textSine = "!!! Cracked by DRS !!!";
char* textTicker =
    "Intently sniff hand thinking about you i'm joking it's food always food. Human is in bath "
    "tub, emergency! "
    "drowning! meooowww!. Furrier and even more furrier hairball sleeping in the box yet naughty "
    "running cat for blow "
    "up sofa in 3 seconds, or get away from me stupid dog meow to be let in one of these days i'm "
    "going to get that "
    "red dot, just you wait and see . Run around the house at 4 in the morning see brother cat "
    "receive pets, attack "
    "out of jealousy growl at dogs in my sleep claw drapes, and play riveting piece on synthesizer "
    "keyboard so i see a "
    "bird i stare at it i meow at it i do a wiggle come here birdy. Reaches under door into "
    "adjacent room demand to be "
    "let outside at once, and expect owner to wait for me as i think about it that box? i can fit "
    "in that box, and go "
    "crazy with excitement when plates are clanked together signalling the arrival of cat food. "
    "Scratch so owner "
    "bleeds cry louder at reflection. Push your water glass on the floor run in circles eat prawns "
    "daintily with a "
    "claw then lick paws clean wash";

int   text_len;
int   textSineX;
int   textSineY;
float modX;
float charY;

double circX = 0;
double circY = 0;

double tickerX = 0;
int    tickerLength;
int    tickerY = 0;

void setup() {
    tv_scale_set( 1 );

    text_len     = strlen( textSine );
    tickerLength = strlen( textTicker ) * 8;
    tickerX      = WIDTH;
    loop_interval_set( 1000000 / 60 );
    textSineX = ( WIDTH - strlen( textSine ) * 8 ) / 2;
    textSineY = HEIGHT / 2;
}

int      note       = 88;
uint16_t colTestCnt = 0;

void loop() {    // need phase inc based on inverval // OR fixed delta based on sec=1000000us
    loop_measure_us( MEASURE_START );

    tv_print( tickerX, HEIGHT - CHAR_HEIGHT, 0x0ff, textTicker );
    tickerX -= loop_delta_get() * 32;
    if ( tickerX < -tickerLength / 2 ) tickerX = WIDTH;

    for ( size_t charX = 0; charX < text_len; charX++ ) {
        char ch = textSine [ charX ];
        modX    = waves_sine12_get( ( sinPosMain + sinPos + charX * 4 ) );
        charY   = ( waves_sine12_get( sinPos + ( 1 / charX ) * 66 ) ) *
                ( waves_sine12_get( sinPosMain + charX * 55 ) );

        int charPosX = WIDTH2 / 2 + WIDTH2 / 2 * waves_sine12_get( circX ) + charX * 8 + modX * 31;
        int charPosY = ( HEIGHT2 - CHAR_HEIGHT * 4 ) + ( HEIGHT2 - CHAR_HEIGHT * 4 ) * charY / 2;
        uint16_t color =
            color12_from_hsl( fabs( ( (float)( charPosY / 2 ) / HEIGHT ) - .5f ) * 3,
                              .5f + waves_sine12_unbiased_get( ( colTestCnt++ ) ) / 2,
                              1 - fabs( ( (float)( charPosX / 2 ) / WIDTH2 ) - .5f ) * 1.9 );

        tv_char( charPosX, charPosY, ch, color );

        sinPos += loop_delta_get() * 33;
        circX += loop_delta_get() * 34;

        sinPosMain += loop_delta_get() * 35;
    }

    // loop_sleep_us(100000);
    tv_print( WIDTH / 2, HEIGHT / 20 * 15, 0xfff, "fps: %f ", 1 / loop_delta_get() );
    tv_print( WIDTH / 2, HEIGHT / 20 * 16, 0xfff, "loop_delta_full_ns %8i ns", loop_delta_full_ns );
    tv_print( WIDTH / 2, HEIGHT / 20 * 17, 0xfff, "loop_lifetime %8i s",
              loop_lifetime_ns / 1000000000 );
    tv_print( WIDTH / 2, HEIGHT / 20 * 18, 0xfff, "Loop needed %8i us",
              loop_measure_us( MEASURE_STOP_AVG ) );
}
