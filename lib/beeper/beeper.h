/**
 * ********************************************
 * \subpage audio
 *  @file beeper.h
 *  @brief ...
 *********************************************/
#pragma once

#include "alsa/asoundlib.h"
#define MIN(i, j) (((i) < (j)) ? (i) : (j))
#define MAX(i, j) (((i) > (j)) ? (i) : (j))

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <math.h>
#include <stdbool.h>


#define SAMPLE_RATE 44100
#define BUFFER_SIZE 4096

#define PITCH_MOD_LINEAR 0
#define PITCH_MOD_SINE 1
#define PITCH_MOD_RANDOM 2

extern void Beeper_setup();
extern void Beeper_exit();
extern void Beeper_audio_write(snd_pcm_sframes_t len, uint8_t** audio_buffer);
extern void Beeper_set(uint8_t ch, uint8_t note, uint8_t pwm);
extern void Beeper_note_fade(
  uint8_t channel, uint8_t noteSrc, uint8_t noteTrg,
  float step_inc, uint16_t step_delay_us, uint16_t length_us
);
extern void Beeper_pwm_set(uint8_t channel, uint8_t pwm);
extern void Beeper_note_set(uint8_t channel, uint8_t note);
