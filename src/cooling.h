#ifndef _TEMPFAN_H_
#define _TEMPFAN_H_

#include "control.h"
#include "defines.h"

#define fanEnableThreshold 2600
#define fanThresholdDeadband 50
#define fanMaxPWM 0
#define fanMinPWM 3000

#define FAN_CTRL_AUTO 0
#define FAN_CTRL_MAN 1

void configureCooling();

void setFanMode(bool mode);
void setFanState(bool state);
bool getFanState();
uint16_t getFanRPM();
uint16_t getFanPWM();
void oneSecondTimerInterrupt();
void fanTachInterruptHandler();

#endif