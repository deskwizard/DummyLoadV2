#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "defines.h"

#define DISPLAY_UPDATE_TIME 500 // Update and flashing rates

#define DISPLAY_MODE_SET 0
#define DISPLAY_MODE_VALUE 1
#define DISPLAY_SET_TIMEOUT 3000

#define INTENSITY_DEFAULT 12
#define INTENSITY_LOW 4

#define DISPLAY_MAIN 0
#define DISPLAY_AUX 1

void configureDisplay();
void displayDashes();
void displayValue(bool displayID, uint16_t value);
void setDisplayMode(bool mode);
bool getDisplayMode();
void selectSetDigit(uint8_t digit);
uint8_t getSelectedDigit();
void handleDisplay();
void resetSetModeTimeout();
void displayAlarmSet(uint8_t error);
void displayAlarmClear();

#endif