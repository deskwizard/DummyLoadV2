#ifndef _INPUT_H_
#define _INPUT_H_

#include "defines.h"

#define ENCODER_STEP 10

#define EN_SW_DOWN false
#define EN_SW_UP true

void configureInputs();
void handleInputs();
void inputTimerInterrupt();

#endif