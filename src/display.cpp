#include "display.h"
#include "cooling.h"
#include "LedControl.h"

// LedControl(int dataPin, int clkPin, int csPin, int numDevices=1);
LedControl display = LedControl(11, 13, 10, 1);

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
    //displaySerial.print(getCurrent());
  } else {
    // displaySerial.print(getCurrentValue());
    if (getOutputState()) {

      //uint16_t voltage = 0; // lol voltage...
      // Todo: Fix that next line
      // uint16_t voltage = uint16_t(getCurrentValue() * (3.3 / 4095.0) * 1000);

      //displaySerial.print(voltage);
    } else {
      // displaySerial.print(0);
      //displaySerial.print(getCurrent());
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
  //displaySerial.write('E');
  //displaySerial.print(error);
}

void displayAlarmClear() { //displaySerial.write('e'); 
}

void serialDisplay() {

  // Serial.print("      Current: ");
  // Serial.print(getCurrentValue());
  // float voltage = getCurrentValue() * (3.3 / 4095.0);
  // Serial.print("      Current: ");
  // Serial.print(voltage, 3);
  // Serial.println();
}
