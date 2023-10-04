#ifndef _TEMPFAN_H_
#define _TEMPFAN_H_

#include "control.h"
#include "defines.h"

#define COOLING_INTERVAL 1000

#define NTC_READ_COUNT 50

// In celcius
#define OVERTEMP_THRESHOLD 40
#define NTC_FAN_THRESHOLD 35
#define FAN_OFF_THRESHOLD 30

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
void setFanPWM(uint16_t value);
void fanTachInterruptHandler();
void oneSecondTimerInterrupt();

/**********  NTC *********/

void readNTC();
uint16_t getNTC();
uint8_t getTemperature();
void printTemperature();

// Constants for abalog read value to degrees C conversion
#define R1 10000
#define C1 1.009249522e-03f
#define C2 2.378405444e-04f
#define C3 2.019202697e-07f

#endif