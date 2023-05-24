#define SEQ_COLS 5
#define SEQ_ROWS 0xff
#define SEQ_PATTERNS 0xf
#define SEQ_SONG_LENGTH 0xff

uint8_t seq_pattern_data[ SEQ_PATTERNS ][ SEQ_ROWS ][ SEQ_COLS ];
uint8_t seq_song_data[ SEQ_SONG_LENGTH ];
uint8_t seq_pattern_length[ SEQ_PATTERNS ];

uint8_t seq_pattern_active;

uint8_t cursor_x, cursor_y;



void seq_pattern_draw(){
  for (size_t x = 0; x < WIDTH/8; x++) { for (size_t y = 0; y < HEIGHT/8; y++) {
    tv_print( 4+x*8, 4+y*8, 0xfff, "%i", (x+y)%9 );
  }}

}

void setup(){
  loop_interval_set(860000);
  tv_scale_set(1);
}

void loop(){
  static uint8_t cc;
  tv_clear(COLORS[16-cc]);
  cc++;
  cc %= 0xf;
  tv_border( 0, 0, ROWS, 2, 0, COLORS[4]);
  tv_print( WIDTH/2, 8, COLORS[cc], "LALALALALALA");
  tv_border( 0, 16, ROWS/3*2+1, LINES-2, 0, COLORS[2]);
  tv_border( (1+ROWS/3*2)*8, 16, ROWS/3, LINES-2, 0, COLORS[10]);
  tv_render();
  //seq_pattern_draw();
}
