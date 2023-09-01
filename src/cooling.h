#ifndef _TEMPFAN_H_
#define  _TEMPFAN_H_

#include "defines.h"
#include "control.h"

#define fanEnableThreshold 2600
#define fanThresholdDeadband 50
#define fanMaxPWM 0
#define fanMinPWM 3000

#define tempReadInterval 5
#define dataPointCount 50
#define NTC_OT_THRESHOLD 2900

#define FAN_CTRL_AUTO 0
#define FAN_CTRL_MAN 1


void handleCooling();
void configureCooling();

void readNTC();
uint16_t getNTCValue();

void setFanMode(bool mode);
void setFanState(bool state);
void setFanSpeed(uint16_t value);
bool getFanState();
uint16_t getFanRPM();
uint16_t getFanPWM();
void oneSecondTimerInterrupt();
void fanTachInterruptHandler();

#endif