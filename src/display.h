#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "defines.h"

#define DISPLAY_UPDATE_TIME 500 // Update and flashing rates

#define DISPLAY_MODE_SET 0
#define DISPLAY_MODE_VALUE 1

#define INTENSITY_DEFAULT 12
#define INTENSITY_LOW 4

void configureDisplay();
void displayDashes();
void displayValue(uint16_t value);
void setDisplayMode(bool mode);
bool getDisplayMode();
void selectSetDigit(uint8_t digit);
uint8_t getSelectedDigit();
void handleDisplay();
void displayAlarmSet(uint8_t error);
void displayAlarmClear();

#endif