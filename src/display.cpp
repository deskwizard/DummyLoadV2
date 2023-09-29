#include "display.h"
#include "LedControl.h"
#include "cooling.h"

// LedControl(int dataPin, int clkPin, int csPin, int numDevices=1);
LedControl display = LedControl(11, 13, 10, 1);

// Needs it global since we want to reset it for "immediate" display update
// when we are adjusting the flashing digits, else we skip "increasing the
// digits" because the update rate is way too slow.
uint32_t previousMillis = 0;

uint8_t displayDigits[4] = {0};
bool periodValues[4] = {false};

bool displayMode = DISPLAY_MODE_VALUE;
uint8_t selectedDigit = 0;

void configureDisplay() {

  display.setScanLimit(0, 4);
  display.shutdown(0, false);
  display.setIntensity(0, INTENSITY_DEFAULT);
  display.clearDisplay(0);

  displayValue(getCurrent());
  // displayDashes();
}

void displayDashes() {
  for (uint8_t x = 0; x <= 3; x++) {
    display.setChar(0, x, '-', false);
  }
}

void displayValue(uint16_t value) {

  // Reset the display update timer so it updates ASAP
  previousMillis = millis();

  displayDigits[0] = (value / 1000U) % 10;
  displayDigits[1] = (value / 100U) % 10;
  displayDigits[2] = (value / 10U) % 10;
  displayDigits[3] = (value / 1U) % 10;
  periodValues[0] = true;

  for (uint8_t x = 0; x <= 3; x++) {
    display.setDigit(0, x, displayDigits[x], periodValues[x]);
  }
}

void setDisplayMode(bool mode) {
  displayMode = mode;
  Serial.println(mode);
  if (displayMode == DISPLAY_MODE_VALUE) {
    displayValue(getCurrent());
  }
}

bool getDisplayMode() { return displayMode; }

void selectSetDigit(uint8_t digit) {
  selectedDigit = digit;
  displayValue(getCurrent());
  Serial.print("selected digit: ");
  Serial.println(selectedDigit);
}

uint8_t getSelectedDigit() { return selectedDigit; }
/*
void updateDisplay2() {

  uint32_t currentMillis = millis();
  static uint32_t previousMillis = 0;
  static bool state = false;

  if ((uint32_t)(currentMillis - previousMillis) >= 500) {
    // Serial.println(state);
    state = !state;
    digitalWrite(pinAlarmLED, state);
    previousMillis = currentMillis;
  }


    if (getDisplayMode() == DISPLAY_MODE_SET) {
      // displaySerial.print(getCurrent());
    } else {
      // displaySerial.print(getCurrentValue());
      if (getOutputState()) {

        // uint16_t voltage = 0; // lol voltage...
        //  Todo: Fix that next line
        //  uint16_t voltage = uint16_t(getCurrentValue() * (3.3 / 4095.0) *
        //  1000);

        // displaySerial.print(voltage);
      } else {

      }
    }

}
*/
void handleDisplay() {

  uint32_t currentMillis = millis();
  // static uint32_t previousMillis = 0;
  static bool displayIntensity = true;

  if ((uint32_t)(currentMillis - previousMillis) >= DISPLAY_UPDATE_TIME) {

    // If we just started, do a little period show
    // we're gonna need a counter and a bit of a mess...

    if (getAlarmFlag()) {
      if (displayIntensity) {
        display.setIntensity(0, INTENSITY_DEFAULT);
      } else {
        display.setIntensity(0, INTENSITY_LOW);
      }
      displayIntensity = !displayIntensity;
    } else {

      for (uint8_t x = 0; x <= 3; x++) {

        Serial.print("here ");
        Serial.print(x);
        Serial.print("  mode: ");
        Serial.print(displayMode);
        Serial.print("  digit: ");
        Serial.println(selectedDigit);

        // We don't have an error, and we're in set mode so flash digit
        if (x == selectedDigit && displayMode == DISPLAY_MODE_SET) {
          if (displayIntensity) {
            //            Serial.print("that one high: ");
            //            Serial.println(x);
            display.setDigit(0, x, displayDigits[x], periodValues[x]);
          } else {
            //            Serial.print("that one low: ");
            display.setChar(0, x, '_', false);
          }
          displayIntensity = !displayIntensity;
        }
        // here
      } // loopy
    }   // else

    previousMillis = currentMillis;
  }
}
/*
// Will need rework for flashing and stuff
void handleDisplay2() {

  uint32_t currentMillis = millis();
  static uint32_t previousMillis = 0;

  if ((uint32_t)(currentMillis - previousMillis) >= DISPLAY_UPDATE_TIME) {
    if (!getAlarmFlag()) {
      updateDisplay();
    }
    serialDisplay();
    previousMillis = currentMillis;
  }
}
*/
void displayAlarmSet(uint8_t error) {}

void displayAlarmClear() { // displaySerial.write('e');
}
