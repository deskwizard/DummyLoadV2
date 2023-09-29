#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "defines.h"

#define DISPLAY_UPDATE_TIME 1000 // in ms

#define DISPLAY_MODE_SET 0
#define DISPLAY_MODE_VALUE 1

#define INTENSITY_DEFAULT 12

void configureDisplay();
void displayDashes();
void setDisplayMode(bool mode);
bool getDisplayMode();
void selectSetDigit(uint8_t digit);
uint8_t getSelectedDigit();
void displayAlarmSet(uint8_t error);
void displayAlarmClear();

void handleDisplay();
void serialDisplay();


#endif