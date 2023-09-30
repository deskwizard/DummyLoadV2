#ifndef _TEMPFAN_H_
#define _TEMPFAN_H_

#include "control.h"
#include "defines.h"

#define fanEnableThreshold 2600
#define fanThresholdDeadband 50
#define fanMaxPWM 0
#define fanMinPWM 3000

#define TACH_PULSE_PER_ROTATION 2

#define FAN_CTRL_AUTO 0
#define FAN_CTRL_MAN 1

void configureCooling();
void handleCooling();

/**********  Fans *********/
void setFanMode(bool mode);
void setFanState(bool state);
bool getFanState();
uint16_t getFanRPM();
uint16_t getFanPWM();
void fanTachInterruptHandler();
void oneSecondTimerInterrupt();

/**********  NTC *********/
#define NTC_READ_COUNT 50
#define MAX_NTC 500

void readNTC();
uint16_t getNTC();
void printTemperature();

// Constants for abalog read value to degrees C conversion
#define R1 10000
#define C1 1.009249522e-03f
#define C2 2.378405444e-04f
#define C3 2.019202697e-07f

#endif