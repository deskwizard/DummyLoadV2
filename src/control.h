#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "defines.h"

#define MCP4725_ADDR 0x60

#define VREF 4.405f
#define MAX_CURRENT 4200 // ;)

#define OUTPUT_ON HIGH
#define OUTPUT_OFF LOW

#define ALARM_FAN_FAIL 0
#define ALARM_OVER_TEMP 1
#define ALARM_OVER_CURRENT 2

void configureControls();
void setOutputState(bool state);
bool getOutputState();
void setAlarm(uint8_t alarmType);
void clearAlarm();
bool getAlarmFlag();
void setCurrent(uint16_t current);
void setDAC(uint16_t value);
uint16_t getCurrent();

#endif