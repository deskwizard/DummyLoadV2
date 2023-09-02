#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "defines.h"

#define DISPLAY_UPDATE_TIME 1000 // in ms

#define DISPLAY_MODE_SET 0
#define DISPLAY_MODE_VALUE 1

void configureDisplay();
void setDisplayMode(bool mode);
bool getDisplayMode();

void handleDisplay();
void serialDisplay();


#endif