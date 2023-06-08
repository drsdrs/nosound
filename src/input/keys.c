
uint16_t lastBtnsPressed;
uint16_t lastBtnsReleased;

uint16_t key_pressed( uint16_t compairBtns ) {
    if ( compairBtns == 0 ) return lastBtnsPressed;
    return ( compairBtns == lastBtnsPressed );
}

uint16_t key_released( uint16_t compairBtns ) {
    if ( compairBtns == 0 ) return lastBtnsReleased;
    return ( compairBtns == lastBtnsReleased );
}

void keys_clear() {
    lastBtnsPressed  = 0;
    lastBtnsReleased = 0;
}

void key_poll() {
    static uint16_t tempBtnValues;
    SDL_Event       e;
    if ( SDL_PollEvent( &e ) == 0 ) return;

    if ( e.type == SDL_QUIT ) {
        printf( "e.type == SDL_QUIT\n" );
        loop_exit();

    } else if ( e.type == SDL_KEYDOWN || e.type == SDL_KEYUP ) {
        static uint16_t lastKeyboardKey;
        SDL_Keymod      key_mod = SDL_GetModState() & 4095;
        tempBtnValues           = 0;
        // printf("key pressed %i - %c\n", e.key.keysym.sym ,e.key.keysym.sym );
        if ( e.key.keysym.sym == 'w' ) {
            tempBtnValues = BTN_UP;
        } else if ( e.key.keysym.sym == 's' ) {
            tempBtnValues = BTN_DOWN;
        } else if ( e.key.keysym.sym == 'd' ) {
            tempBtnValues = BTN_RIGHT;
        } else if ( e.key.keysym.sym == 'a' ) {
            tempBtnValues = BTN_LEFT;
        } else if ( e.key.keysym.sym == ' ' ) {
            tempBtnValues = BTN_A;
        } else if ( key_mod == KMOD_LSHIFT && e.key.keysym.sym == 27 ) {
            printf( "Quit key SHIFT+ESC\n" );
            loop_exit();
            // exit(0);
        } else if ( key_mod == KMOD_LCTRL && e.key.keysym.sym == 27 ) {
            printf( "GOTO prgManagerUI CTRL+ESC\n" );
            loop_exit();
            prg_change( "prgManagerUI" );
        }

        if ( e.type == SDL_KEYDOWN && lastKeyboardKey != tempBtnValues ) {
            lastKeyboardKey = tempBtnValues;
            lastBtnsPressed |= tempBtnValues;
        } else if ( e.type == SDL_KEYUP ) {
            lastKeyboardKey = 0;
            lastBtnsReleased |= tempBtnValues;
        }

        // SDL_GetModState()-4096;
    } else if ( e.type == SDL_JOYBUTTONDOWN || e.type == SDL_JOYBUTTONUP ) {
        tempBtnValues = 0;
        if ( SDL_JoystickGetButton( gamepad, 0 ) ) {
            tempBtnValues = BTN_A;
        } else if ( SDL_JoystickGetButton( gamepad, 1 ) ) {
            tempBtnValues = BTN_B;
        } else if ( SDL_JoystickGetButton( gamepad, 2 ) ) {
            tempBtnValues = BTN_X;
        } else if ( SDL_JoystickGetButton( gamepad, 3 ) ) {
            tempBtnValues = BTN_Y;
        } else if ( SDL_JoystickGetButton( gamepad, 4 ) ) {
            tempBtnValues = BTN_S_LEFT;
        } else if ( SDL_JoystickGetButton( gamepad, 5 ) ) {
            tempBtnValues = BTN_S_RIGHT;
        } else if ( SDL_JoystickGetButton( gamepad, 6 ) ) {
            tempBtnValues = BTN_SELECT;
        } else if ( SDL_JoystickGetButton( gamepad, 7 ) ) {
            tempBtnValues = BTN_START;
        }

        if ( e.type == SDL_JOYBUTTONDOWN ) {
            lastBtnsPressed |= tempBtnValues;
        } else if ( e.type == SDL_JOYBUTTONUP ) {
            lastBtnsReleased |= tempBtnValues;
        }

    } else if ( e.type == SDL_JOYAXISMOTION ) {    // Motion on controller 0
        static uint8_t lastDirX = 0;
        static uint8_t lastDirY = 0;
        if ( e.jaxis.which == 0 ) {
            if ( e.jaxis.axis == 0 ) {    //  LEFT - RIGHT
                if ( e.jaxis.value < -JOYSTICK_DEAD_ZONE ) {
                    lastBtnsPressed = BTN_LEFT;
                    lastDirX        = BTN_LEFT;
                } else if ( e.jaxis.value > JOYSTICK_DEAD_ZONE ) {
                    lastBtnsPressed = BTN_RIGHT;
                    lastDirX        = BTN_RIGHT;
                } else {
                    lastBtnsReleased = lastDirX;
                    lastDirX         = 0;
                }
            } else if ( e.jaxis.axis == 1 ) {                   // UP - DOWN
                if ( e.jaxis.value < -JOYSTICK_DEAD_ZONE ) {    // Below of dead zone
                    lastBtnsPressed = BTN_UP;
                    lastDirY        = BTN_UP;
                } else if ( e.jaxis.value > JOYSTICK_DEAD_ZONE ) {
                    lastBtnsPressed = BTN_DOWN;
                    lastDirY        = BTN_DOWN;
                } else {
                    lastBtnsReleased |= lastDirY;
                    lastDirY = 0;
                }
            }
        }
    }
}
