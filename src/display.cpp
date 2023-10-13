#include "display.h"
#include "LedControl.h"
#include "cooling.h"

// LedControl(int dataPin, int clkPin, int csPin, int numDevices=1);
LedControl display = LedControl(PIN_MOSI, PIN_SCK, DISPLAY_CS, 1);

// Needs it global since we want to reset it for "immediate" display update
// when we are adjusting the flashing digits, else we skip "increasing the
// digits" because the update rate is way too slow.
uint32_t previousMillis = 0;
uint32_t displayTimeoutMillis = 0;

uint8_t displayDigits[4] = {0};
bool periodValues[4] = {false};

bool displayMode = DISPLAY_MODE_VALUE;
uint8_t selectedDigit = 0;

void configureDisplay() {

  display.setScanLimit(0, 4);
  display.shutdown(0, false);
  display.setIntensity(0, INTENSITY_DEFAULT);
  display.clearDisplay(0);

  displayDashes();
  delay(500);

  displayValue(getCurrent());
}

void displayDashes() {
  for (uint8_t x = 0; x <= 3; x++) {
    display.setChar(0, x, '-', false);
  }
}

void displayValue(uint16_t value) {

  // Reset the display update timer so it updates ASAP
  // Todo: That doesn't work as expected...
  previousMillis = 0;

  if (getOutputRange() == RANGE_LOW) {
    value = value / 10;
  }

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
  // Todo: reset the display update thing here?
  selectedDigit = digit;
  displayValue(getCurrent());
  Serial.print("selected digit: ");
  Serial.println(selectedDigit);
}

uint8_t getSelectedDigit() { return selectedDigit; }

void handleDisplay() {

  uint32_t currentMillis = millis();
  static bool displayIntensity = true;

  if ((uint32_t)(currentMillis - previousMillis) >= DISPLAY_UPDATE_TIME) {

    // debug
    // printTemperature();

    if (getAlarmFlag()) {
      if (displayIntensity) {
        display.setIntensity(0, INTENSITY_DEFAULT);
      } else {
        display.setIntensity(0, INTENSITY_LOW);
      }
      displayIntensity = !displayIntensity;

    } // Otherwise set mode
    else if (displayMode == DISPLAY_MODE_SET) {

      if (millis() - displayTimeoutMillis >= DISPLAY_SET_TIMEOUT &&
          displayTimeoutMillis != 0) {

        displayTimeoutMillis = 0;
        setDisplayMode(DISPLAY_MODE_VALUE);
      }

      for (uint8_t x = 0; x <= 3; x++) {

        // We don't have an error and we're in set mode, so flash selected digit
        // (displayIntensity is just reused here)
        if (x == selectedDigit) {
          if (displayIntensity) {
            display.setDigit(0, x, displayDigits[x], periodValues[x]);
          } else {
            display.setChar(0, x, '_', false);
          }
          displayIntensity = !displayIntensity;
        }

      } // for
    }   // else if

    previousMillis = currentMillis;
  }
}

void resetSetModeTimeout() {
  displayTimeoutMillis = millis();
  Serial.println("reset display timeout");
}

void displayAlarmSet(uint8_t alarmType) {
  //
  display.setChar(0, 0, 'E', false);

  display.setRow(0, 1, 0b0000101); // 'r'
  display.setRow(0, 2, 0b0000101);

  if (alarmType == ALARM_FAN_FAIL) {
    display.setChar(0, 3, 'F', false);
  } else if (alarmType == ALARM_OVER_TEMP) {
    display.setChar(0, 3, 'H', false);
  } else if (alarmType == ALARM_OVER_CURRENT) {
    display.setChar(0, 3, 'C', false);
  }
}

void displayAlarmClear() {
  display.setIntensity(0, INTENSITY_DEFAULT);
  displayValue(getCurrent());
}
