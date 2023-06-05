#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "alsa.h"
#include "alsa/asoundlib.h"
#include "beeper.h"

snd_timer_t *handle;
uint8_t     *alsa_audio_buffer;
int          err;
snd_pcm_t   *pcm;
long         samples_per_timer_periode;
long         framesMust        = 0;
uint8_t      alsa_running_flag = false;

void alsa_stop() {
    printf( "alsa_stop alsa_running_flag: %i\n", alsa_running_flag );
    if ( alsa_running_flag == false ) return;
    snd_pcm_close( pcm );
    snd_timer_stop( handle );
    snd_timer_close( handle );
    alsa_running_flag = false;
}

static void alsa_write( int nframes, uint8_t *samples ) {
    // printf("%li\n", nframes);
    //  if(alsa_running_flag==true)
    snd_pcm_writei( pcm, samples, nframes );
    // if ((err = snd_pcm_writei(pcm, samples, nframes)) < 0) {
    //     fprintf(stderr, "write failed (%s)\n", snd_strerror(err));
    //     exit(1);
    // }
    // else { fprintf(stderr, "write succesfully (%s)\n", snd_strerror(err)); }
}

void async_callback( snd_async_handler_t *ahandler ) {
    // tv_key_poll(0);
    snd_timer_t      *handle = snd_async_handler_get_timer( ahandler );
    snd_timer_read_t  tr;
    snd_pcm_sframes_t frames_to_deliver;

    if ( snd_timer_read( handle, &tr, sizeof( tr ) ) == sizeof( tr ) ) {
        framesMust += samples_per_timer_periode * tr.ticks;

        if ( ( frames_to_deliver = snd_pcm_avail_update( pcm ) ) <
             0 ) { /* find out how much space is available for playback data */
            if ( frames_to_deliver == -EPIPE ) {
                fprintf( stderr, "an xrun occured\n" );
                int err;
                if ( ( err = snd_pcm_prepare( pcm ) ) < 0 ) {
                    fprintf( stderr, "cannot prepare audio interface for use (%s)\n",
                             snd_strerror( err ) );
                    exit( 1 );
                }
                if ( ( err = snd_pcm_recover( pcm, frames_to_deliver, 0 ) ) < 0 ) {
                    fprintf( stderr, "cannot snd_pcm_recover. (%s)\n", snd_strerror( err ) );
                    exit( 1 );
                }
                framesMust = 0;
                Beeper_audio_write( 4096, &alsa_audio_buffer );
                alsa_write( 4096, alsa_audio_buffer );
                // snd_async_add_timer_handler(&ahandler, handle, async_callback, 0);

            } else {
                fprintf( stderr, "unknown ALSA avail update return value (%ld)\n",
                         frames_to_deliver );
            }
            return;
            // exit(EXIT_FAILURE);
        }
        frames_to_deliver = frames_to_deliver > framesMust ? framesMust : frames_to_deliver;
        frames_to_deliver = frames_to_deliver > 4096 ? 4096 : frames_to_deliver;
        framesMust -= frames_to_deliver;
        if ( framesMust != 0 ) fprintf( stderr, "framesMust (%li)\n", framesMust );
        if ( frames_to_deliver == 0 ) return;
        Beeper_audio_write( frames_to_deliver, &alsa_audio_buffer );
        alsa_write( frames_to_deliver, alsa_audio_buffer );
        // printf("TIMER-ASYNC: resolution = %uns, ticks = %u\n",	tr.resolution, tr.ticks);
    }
}

int  alsa_setup() {
    alsa_audio_buffer = (uint8_t *)malloc( 4096*sizeof( uint8_t ) );
    memset( alsa_audio_buffer, 127 /*silence*/, 4096 );

    snd_pcm_hw_params_t *hw_params;
    snd_pcm_sw_params_t *sw_params;
    snd_pcm_hw_params_alloca( &hw_params );

    if ( snd_pcm_open( &pcm, "default", 0, 0 ) ) {
        fprintf( stderr, "snd_pcm_open ERROR(%s)\n", snd_strerror( err ) );
        exit( 1 );
    }
    snd_pcm_set_params( pcm, SND_PCM_FORMAT_U8, SND_PCM_ACCESS_RW_INTERLEAVED, 1, SAMPLE_RATE, 0,
                        -1 );

    if ( ( err = snd_pcm_prepare( pcm ) ) < 0 ) {
        fprintf( stderr, "cannot prepare audio interface for use (%s)\n", snd_strerror( err ) );
        exit( 1 );
    }

    if ( ( err = snd_pcm_sw_params_malloc( &sw_params ) ) < 0 ) {
        fprintf( stderr, "cannot allocate software parameters structure (%s)\n",
                 snd_strerror( err ) );
        exit( 1 );
    }
    if ( ( err = snd_pcm_sw_params_current( pcm, sw_params ) ) < 0 ) {
        fprintf( stderr, "cannot initialize software parameters structure (%s)\n",
                 snd_strerror( err ) );
        exit( 1 );
    }
    if ( ( err = snd_pcm_sw_params_set_avail_min( pcm, sw_params, 367 ) ) < 0 ) {
        fprintf( stderr, "cannot set minimum available count (%s)\n", snd_strerror( err ) );
        exit( 1 );
    }
    if ( ( err = snd_pcm_sw_params_set_start_threshold( pcm, sw_params, 367 ) ) < 0 ) {
        fprintf( stderr, "cannot set start mode (%s)\n", snd_strerror( err ) );
        exit( 1 );
    }

    if ( ( err = snd_pcm_sw_params_set_stop_threshold( pcm, sw_params, 367 * 2 ) ) < 0 ) {
        fprintf( stderr, "cannot set snd_pcm_sw_params_set_stop_threshold mode (%s)\n",
                 snd_strerror( err ) );
        exit( 1 );
    }

    if ( ( err = snd_pcm_sw_params_set_silence_threshold( pcm, sw_params, 367 * 2 ) ) < 0 ) {
        fprintf( stderr, "cannot set snd_pcm_sw_params_set_silence_threshold mode (%s)\n",
                 snd_strerror( err ) );
        exit( 1 );
    }

    if ( ( err = snd_pcm_sw_params( pcm, sw_params ) ) < 0 ) {
        fprintf( stderr, "cannot set software parameters (%s)\n", snd_strerror( err ) );
        exit( 1 );
    }

    for ( size_t i = 0; i < 0x08; i++ ) alsa_write( 409, alsa_audio_buffer );    //   PRE-BUFFERING

    /* ###########################/
    --      SETUP TIMER       ---/
    ###########################*/

    int class     = SND_TIMER_CLASS_GLOBAL;
    int sclass    = SND_TIMER_CLASS_NONE;
    int card      = 0;
    int device    = SND_TIMER_GLOBAL_SYSTEM;
    int subdevice = 0;

    snd_timer_id_t      *id;
    snd_timer_info_t    *info;
    snd_timer_params_t  *params;
    char                 timername[64];
    snd_async_handler_t *ahandler;

    snd_timer_id_alloca( &id );
    snd_timer_info_alloca( &info );
    snd_timer_params_alloca( &params );

    if ( class == SND_TIMER_CLASS_SLAVE && sclass == SND_TIMER_SCLASS_NONE ) {
        fprintf( stderr, "slave class is not set\n" );
        exit( EXIT_FAILURE );
    }

    sprintf( timername, "hw:CLASS=%i,SCLASS=%i,CARD=%i,DEV=%i,SUBDEV=%i", class, sclass, card,
             device, subdevice );
    if ( ( err = snd_timer_open( &handle, timername, SND_TIMER_OPEN_NONBLOCK ) ) < 0 ) {
        fprintf( stderr, "timer open %i (%s)\n", err, snd_strerror( err ) );
        exit( EXIT_FAILURE );
    }
    printf( "Using timer class %i, slave class %i, card %i, device %i, subdevice %i\n", class,
            sclass, card, device, subdevice );
    if ( ( err = snd_timer_info( handle, info ) ) < 0 ) {
        fprintf( stderr, "timer info %i (%s)\n", err, snd_strerror( err ) );
        exit( 0 );
    }

    snd_timer_params_set_auto_start( params, 1 );

    if ( !snd_timer_info_is_slave( info ) ) {
        long timer_periode_us     = snd_timer_info_get_resolution( info );
        long ticks_per_sec        = 1000000000 / timer_periode_us;
        long samples_per_sec      = SAMPLE_RATE;
        samples_per_timer_periode = ( samples_per_sec / ticks_per_sec );
        printf( "samples_per_timer_periode %li \n", samples_per_timer_periode );
        snd_timer_params_set_ticks( params, 4 );
    }

    if ( ( err = snd_timer_params( handle, params ) ) < 0 ) {
        fprintf( stderr, "timer params %i (%s)\n", err, snd_strerror( err ) );
        exit( 0 );
    }

    alsa_running_flag = true;
    printf( "alsa_running_flag: %i\n", alsa_running_flag );
    err = snd_async_add_timer_handler( &ahandler, handle, async_callback, 0 );

    if ( err < 0 ) {
        fprintf( stderr, "unable to add async handler %i (%s)\n", err, snd_strerror( err ) );
        exit( EXIT_FAILURE );
    }

    if ( ( err = snd_timer_start( handle ) ) < 0 ) {
        fprintf( stderr, "timer start %i (%s)\n", err, snd_strerror( err ) );
        exit( EXIT_FAILURE );
    }
    return 0;
}
