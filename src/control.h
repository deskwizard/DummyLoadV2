#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "defines.h"

#define OUTPUT_ON HIGH
#define OUTPUT_OFF LOW

#define ALARM_FAN_FAIL 0
#define ALARM_OVER_TEMP 1
#define ALARM_OVER_CURRENT 2

#define maxDAC 3000

void configureControls();
void setOutputState(bool state);
bool getOutputState();
void setAlarm(uint8_t alarmType);
bool getAlarmFlag();
void clearAlarm();

void setDAC(uint16_t value);
uint16_t getValueDAC();

#endif