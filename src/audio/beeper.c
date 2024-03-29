
#include <SDL2/SDL.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "alsaHelper.h"
#include "beeper.h"
#include "waves.h"

#define PITCH_MOD_LINEAR 0
#define PITCH_MOD_SINE   1
#define PITCH_MOD_RANDOM 2

pthread_t        ptid[8];    // no volatile needed ! only if in async
volatile uint8_t async_runnig[8];

int   waveType      = 0;
float waveTone      = 440;
float samplingIndex = 0;

uint8_t paused  = 0;
uint8_t volume  = 8;
uint8_t silence = 127;
char    chState[8];

uint32_t chFreq[8];
uint32_t chCntOff[8];
uint32_t chCntOn[8];
uint16_t chPwm[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };

uint16_t notePhase[8 * 16 + 1];
#define notesPerOctave 16

double noteDivider[notesPerOctave] = {
    0,                  //      0   UNISON
    (double)1 / 15,     //      0#  SEMITONE
    (double)1 / 8,      //      1   WHOLE THIRD
    (double)1 / 5,      //      1#  MINOR THIRD
    (double)2 / 8,      //      2   MAJOR THIRD
    (double)2 / 7,      //      2#  ? MISSING KEY ? BLACK KEY
    (double)1 / 3,      //      3   PERFECT FORTH
    (double)2 / 5,      //      3#  TRITONE
    (double)1 / 2,      //      4   PERFECT FIFTH   =   1.5
    (double)3 / 5,      //      4#  MINOR SIXTH
    (double)2 / 3,      //      5   MAJOR SIXT
    (double)5 / 7,      //      5#  ? MISSING KEY ? BLACK KEY
    (double)6 / 8,      //      6   ? MISSING KEY ? WHITE KEY
    (double)4 / 5,      //      6#  MINOR SEVENTH
    (double)7 / 8,      //      7   MAJOR SEVENTH
    (double)14 / 15,    //      7#  MISSING KEY ? BLACK KEY
                        //(double)1/1
};

void makeFrequencys() {
    int index;
    for ( int oct = 0; oct < 8; oct++ ) {
        for ( int note = 0; note < notesPerOctave; note++ ) {
            index            = note + ( oct * notesPerOctave );
            double noteFreq  = 4.4 * ( ( 1 << oct ) + ( noteDivider[note] * ( 1 << oct ) ) );
            notePhase[index] = (uint32_t)( SAMPLE_RATE / noteFreq );
            // notePhase [ index + 127 ] = (uint32_t)( SAMPLE_RATE / noteFreq );
            //  printf("%i: freq=%i\n", index, notePhase[index]);
        }
    }
}

uint16_t getNotePhase( uint8_t oct, uint8_t note ) {
    return notePhase[note + oct * notesPerOctave];
}

void setFreqPwm( int ch ) {
    if ( chFreq[ch] == 0 || chPwm[ch] == 0 ) {    // TODO!!! check if check is needed
        chCntOff[ch] = 0;
        chCntOn[ch]  = 0;
    } else {                                       //  TODO make pwm act proportional
        chCntOff[ch] = chFreq[ch] >> chPwm[ch];    // MAX( chPwm[ch], chFreq[ch] -1);
        chCntOn[ch]  = chFreq[ch] - chCntOff[ch];
    }
}

void Beeper_audio_write( snd_pcm_sframes_t len, uint8_t** audio_buffer ) {
    static uint8_t tmpBuffer[4096];
    static uint8_t out = 1;
    for ( int i = 0; i < len; i += 1 ) {
        for ( int ch = 0; ch < 8; ch++ ) {
            if ( chFreq[ch] == 0 || chPwm[ch] == 0 ) continue;
            if ( chState[ch] == 0 ) {
                if ( !chCntOn[ch]-- ) {
                    chState[ch] = 1;
                    out ^= 1;
                }
            } else {
                if ( !chCntOff[ch]-- ) {
                    chState[ch] = 0;
                    out ^= 1;
                    setFreqPwm( ch );
                }
            }
        }
        if ( out ) {
            tmpBuffer[i] = silence - volume;
        } else {
            tmpBuffer[i] = silence;
        }
        *audio_buffer = (unsigned char*)&tmpBuffer;
        // audio_buffer[i] = t&t>>8;
        // t++;
    }
}

void Beeper_note_set( uint8_t channel, uint8_t note ) {
    if ( chFreq[channel] != notePhase[note] ) {
        chFreq[channel] = notePhase[note];
        setFreqPwm( channel );
    }
}

void Beeper_pwm_set( uint8_t channel, uint8_t pwm ) {
    if ( chPwm[channel] != pwm ) chPwm[channel] = pwm;
}

void Beeper_set( uint8_t channel, uint8_t note, uint8_t pwm ) {    // 3bit 7bit 4bit = 7-127-15 = 14bit
    Beeper_pwm_set( channel, pwm );
    Beeper_note_set( channel, note );
}

struct async_args {
    uint8_t  channel;
    uint8_t  note_start;
    uint8_t  note_end;
    float    step_inc;
    uint16_t step_delay_us;
    uint16_t length_us;
    uint8_t  type;
};

void* async( void* arguments ) {
    float    mod_state     = 0;
    uint16_t total_time_us = 0;
    float    tempVal;

    struct async_args* args = arguments;
    // printf( "Start sync thread ch: %i\n" ,args->channel);
    async_runnig[args->channel] = true;
    // printf("args %f - %i - %i\n", args->step_inc, args->step_delay_us, args->channel);

    pthread_setcancelstate( PTHREAD_CANCEL_ENABLE, NULL );
    pthread_setcanceltype( PTHREAD_CANCEL_ASYNCHRONOUS, NULL );
    pthread_detach( pthread_self() );

	Beeper_note_set( args->channel, args->note_start );
	Beeper_pwm_set( args->channel, 1 );
    while ( total_time_us < args->length_us ) {

        switch ( args->type ) {
            case PITCH_MOD_LINEAR:
                chFreq[args->channel] =
                    notePhase[args->note_start] * ( 1 - mod_state ) + notePhase[args->note_end] * mod_state;
                if ( mod_state >= 1 )
                    total_time_us = args->length_us;    // break out of main while loop
                break;
            case PITCH_MOD_SINE:
                tempVal               = waves_sine12_get( (uint16_t)( mod_state * 0xfff ) );
                chFreq[args->channel] = notePhase[args->note_start] * ( 1 - tempVal ) +
                                        notePhase[args->note_end] * tempVal;
                break;
            case PITCH_MOD_RANDOM:
                if ( mod_state > 1 ) mod_state--;
                tempVal               = (float)( ( rand() & 0xfff ) * mod_state ) / 0xfff;
                chFreq[args->channel] = notePhase[args->note_start] * ( 1 - tempVal ) +
                                        notePhase[args->note_end] * tempVal;
                break;
        }

        SDL_Delay( args->step_delay_us );
        total_time_us += args->step_delay_us;
        mod_state += args->step_inc;
    }
    chFreq[args->channel] = 0;
    Beeper_pwm_set( args->channel, 0 );
    // printf( "EXIT sync thread ch: %i\n" ,args->channel);
    async_runnig[args->channel] = false;
    pthread_exit( NULL );
}

// play sample with step_inc=1 step_delay_us=samplespeed

// TODO step_delay_us==0 make tone play endless
void Beeper_note_fade( uint8_t channel, uint8_t note_start, uint8_t note_end, float step_inc, uint16_t step_delay_us, uint16_t length_us ) {
    struct async_args* args = (struct async_args*)malloc( sizeof( struct async_args ) );
    args->channel           = channel;
    args->note_start        = note_start;
    args->note_end          = note_end;
    args->step_inc          = step_inc;
    args->step_delay_us     = step_delay_us;
    args->length_us         = length_us;
    args->type              = PITCH_MOD_SINE;

    if( args->step_delay_us <= 0 ) args->step_delay_us = 1;

    if ( async_runnig[channel] == true ) {
        // printf("STOP-STOP sync thread %i\n", async_runnig[args->channel]);
        pthread_cancel( ptid[channel] );
        async_runnig[channel] = false;
    }
    pthread_create( &ptid[channel], NULL, &async, (void*)args );
}

void Beeper_setup() {
    makeFrequencys();
    alsa_setup();
}

void Beeper_exit() {
    alsa_stop();
}
