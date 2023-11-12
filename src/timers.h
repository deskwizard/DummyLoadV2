#ifndef _TIMERS_H_
#define _TIMERS_H_

#include "defines.h"

// Rotary Encoders
#define ENC_PIN PINB
#define ENC_DDR DDRB
#define ENC_PORT PORTB
#define ENC1_A PB0
#define ENC1_B PB1

void configureTimer();
void handleTimers();

#endif
