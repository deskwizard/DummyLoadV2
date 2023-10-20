#ifndef _INPUT_H_
#define _INPUT_H_

#include "defines.h"

#define SW_DOWN false
#define SW_UP true

void configureInputs();
void handleInputs();
void inputTimerInterrupt();

#endif