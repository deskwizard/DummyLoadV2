#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "defines.h"

#define RELAY_DELAY 100 //in ms

#define VREF 4.098f

#define MAX_CURRENT 4000

#define ADC_VOLTAGE_CHANNEL 2
#define ADC_CURRENT_CHANNEL 3

#define ANALOG_READ_COUNT 10
#define ANALOG_READ_TIME 50

#define OUTPUT_ON HIGH
#define OUTPUT_OFF LOW

#define RANGE_LOW 1
#define RANGE_HIGH 0

#define ALARM_FAN_FAIL 0
#define ALARM_OVER_TEMP 1
#define ALARM_OVER_CURRENT 2

void configureControls();
void handleAnalog();
void setOutputState(bool state);
bool getOutputState();
void setOutputRange(bool value);
bool getOutputRange();
void setAlarm(uint8_t alarmType);
void clearAlarm();
bool getAlarmFlag();
void setCurrent(uint16_t current);
void setDAC(uint16_t value);
void readCurrent();
uint16_t getCurrent();
uint16_t getMaxCurrent();
void readVoltage();
uint16_t getVoltage();
uint16_t readChannelSE(uint8_t channel);
void testDAC();

#endif