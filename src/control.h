#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "defines.h"

#define MCP4725_ADDR 0x60

#define VREF 3.317f
#define MAX_CURRENT 3000

#define OUTPUT_ON HIGH
#define OUTPUT_OFF LOW

#define ALARM_FAN_FAIL 0
#define ALARM_OVER_TEMP 1
#define ALARM_OVER_CURRENT 2

//#define maxDAC 3000

void configureControls();
void setOutputState(bool state);
bool getOutputState();
void setAlarm(uint8_t alarmType);
bool getAlarmFlag();
void clearAlarm();

void setCurrent(uint16_t current);
void setDAC(uint16_t value);
uint16_t getCurrent();
//void setOutput(uint16_t desiredOutput);
//uint16_t getValueDAC();
//uint16_t getCurrentValue();
//void readCurrent();

#endif