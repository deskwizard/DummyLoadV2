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
uint8_t selectedDigit = 1;

void configureDisplay() {

  display.setScanLimit(0, 4);
  display.shutdown(0, false);
  display.setIntensity(0, INTENSITY_DEFAULT);
  display.clearDisplay(0);

  displayDashes();
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
  if (displayMode == DISPLAY_MODE_VALUE) {
  }
}

bool getDisplayMode() { return displayMode; }

void selectSetDigit(uint8_t digit) {
  selectedDigit = digit;
  Serial.print("selected digit: ");
  Serial.println(selectedDigit);
}

uint8_t getSelectedDigit() { return selectedDigit; }

void updateDisplay() {

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
      // displaySerial.print(0);
      // displaySerial.print(getCurrent());
    }
  }
}

void handleDisplay() {

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

void displayAlarmSet(uint8_t error) {
  // displaySerial.write('E');
  // displaySerial.print(error);
}

void displayAlarmClear() { // displaySerial.write('e');
}

void serialDisplay() {

  // Serial.print("      Current: ");
  // Serial.print(getCurrentValue());
  // float voltage = getCurrentValue() * (3.3 / 4095.0);
  // Serial.print("      Current: ");
  // Serial.print(voltage, 3);
  // Serial.println();
}
