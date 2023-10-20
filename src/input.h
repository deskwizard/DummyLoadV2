#ifndef _INPUT_H_
#define _INPUT_H_

#include "defines.h"

#define SW_DOWN false
#define SW_UP true

extern volatile bool enableSwitchState;
extern volatile bool rangeSwitchState;
extern volatile bool encoderSwitchState;
extern volatile uint8_t currentEncoderPosition;

void configureInputs();
void handleInputs();
void inputTimerInterrupt();

#endif