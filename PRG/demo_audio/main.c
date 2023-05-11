
int pwm = 0;
int note = 64;
float fadeState = 0;
void setup(){
  loop_interval_set(1000000 / 8.f);
  //Beeper_note_fade( 0, 88, 89, .01 );
}

void loop(){ // need phase inc based on inverval // OR fixed delta based on sec=1000000us
  //Beeper_set(1, 64, 1);
  tv_clear(0x911);
  //Beeper_pwm_set(0, pwm);
  Beeper_note_fade( 0, note, 127, .015f, 13, 8550 );

  // fadeState += 0.001;
  // if( fadeState>1 ) --fadeState;

  //Beeper_pwm_set(0, pwm);
  tv_print( WIDTH/2, HEIGHT/2, 0xfff, "PWM: %i   NOTE: %i   FADE: %f", pwm, note&0x7F, waves_sine12_unbiased_get(pwm));
  pwm += 1;
  if(pwm==7) note++;
  pwm &= 7;
  tv_render();
}
