#define SEQ_CHANNELS    4
#define SEQ_COLS        4
#define INSTR_COLS      7
#define SEQ_ROWS        0xff
#define SEQ_PATTERNS    0xf
#define SEQ_SONG_LENGTH 0xff

uint8_t seq_pattern_data[SEQ_PATTERNS][SEQ_ROWS][SEQ_COLS];
uint8_t seq_song_data[SEQ_SONG_LENGTH];
uint8_t seq_pattern_length[SEQ_PATTERNS];

uint8_t seq_pattern_active;
// channel 1-8 defined by seq_pattern col
// OR by instr
static char* pattern_header_text[SEQ_COLS] = { "Instrument", "Oct", "Note", "Volume" };
static char* instr_header_text[INSTR_COLS] = { "Instrument", "channel", "Type", "Depth", "Speed", "Length", "Delay" };
// inst config only makes sense if multiple channels are used in one instr ?!

uint8_t cursor_pos[3][2];

#define PATTERN_VIEW    0
#define INSTRUMENT_VIEW 1
#define SONG_VIEW       2

uint8_t active_view = PATTERN_VIEW;

void intrument_view_header_draw() {
}
void intrument_view_draw() {
}

void seq_pattern_header_draw() {
    print( 4, 4, 0xfff, "Show selected header Text " );
    for ( size_t ch = 0; ch < SEQ_CHANNELS; ch++ ) {
        for ( size_t x = 0; x < SEQ_COLS; x++ ) {
            int xPos = ( ch * SEQ_COLS * CHAR_HEIGHT ) + x * ( CHAR_HEIGHT - 1 ) + CHAR_HEIGHT * 4;
            // print( xPos, 4, 0xfff, "%c", pattern_header_text[x][0]);
        }
    }
}

void seq_pattern_draw() {
	const int selected_row_color = 0xff0;
	const int selected_cell_color = 0xf00;
	const int empty_color = 0xfff;
    int col = empty_color;
    for ( size_t y = 0; y < LINES - 4; y++ ) {
        int yPos = y * CHAR_HEIGHT + CHAR_HEIGHT2 * 4.5;
        if ( cursor_pos[active_view][1] == y ) col = selected_row_color;
        print( CHAR_HEIGHT2, yPos, col, "%2X:", y );
        for ( size_t ch = 0; ch < SEQ_CHANNELS; ch++ ) {
            for ( size_t x = 0; x < SEQ_COLS; x++ ) {
                if ( cursor_pos[active_view][0] == ch * SEQ_COLS + x && cursor_pos[active_view][1] == y ) col = selected_cell_color;
                int xPos = ( ch * SEQ_COLS * CHAR_HEIGHT ) + x * ( CHAR_HEIGHT - 1 ) + CHAR_HEIGHT * 4;
                print( xPos, yPos, col, "%1X", x );
                if(cursor_pos[active_view][0]== ch * SEQ_COLS + x && cursor_pos[active_view][1] != y ) col = empty_color;
                if(cursor_pos[active_view][1] == y ) col = selected_row_color;
            }
        }
        if ( cursor_pos[active_view][1] == y ) col = empty_color;
    }
}

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
    static float stepSize;
    static int   stepDelay;
    static int   noteBegin;
    static int   noteEnd;
    if ( ticks < ticksPerBeat ) {
        ticks++;
    } else {
        beats++;
        ticks = 0;
    }

    if ( key_pressed( BTN_UP ) ) {
        cursor_pos[active_view][1]--;
        if ( cursor_pos[active_view][1] < 0 ) cursor_pos[active_view][1] = 0xf;    // TODO find max Y value for each view
    } else if ( key_pressed( BTN_DOWN ) ) {
        cursor_pos[active_view][1]++;
        if ( cursor_pos[active_view][1] >= 0xf ) cursor_pos[active_view][1] = 0;
    } else if ( key_pressed( BTN_LEFT ) ) {
        cursor_pos[active_view][0]--;
        if ( cursor_pos[active_view][0] < 0 ) cursor_pos[active_view][0] = 0xf;
    } else if ( key_pressed( BTN_RIGHT ) ) {
        cursor_pos[active_view][0]++;
        if ( cursor_pos[active_view][0] >= 0xf ) cursor_pos[active_view][0] = 0;
    } else if ( key_pressed( BTN_A ) ) {
        Beeper_note_fade( 0, 127, 0, 0.001, 8, 750 );
    }

    border( 0, 0, ROWS, 2, 0, COLORS[4] );

    print_center( WIDTH / 2, CHAR_HEIGHT, COLORS[beats % 0xf], "%2.i, %3.i", ticks, beats );
    border( 0, 16, ROWS / 3 * 2 + 1, LINES - 2, 0, COLORS[2] );
    border( ( 1 + ROWS / 3 * 2 ) * 8, 16, ROWS / 3, LINES - 2, 0, COLORS[10] );
    seq_pattern_draw();
    seq_pattern_header_draw();
}
