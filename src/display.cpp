#include "display.h"
#include "cooling.h"

HardwareSerial displaySerial(displaySerialRX, displaySerialTX);
bool displayMode = DISPLAY_MODE_VALUE;
uint8_t selectedDigit = 0;

void configureDisplay() {

  displaySerial.setTimeout(200);
  displaySerial.begin(115200);
  displaySerial.write('s');
  displaySerial.write('-');
  displaySerial.write('z');
  displaySerial.write('e');
}

void setDisplayMode(bool mode) {
  displayMode = mode;
  if (displayMode == DISPLAY_MODE_VALUE) {
    displaySerial.write('z');
  }
}

bool getDisplayMode() { return displayMode; }

void selectSetDigit(uint8_t digit) {
  selectedDigit = digit;
  Serial.print("selected digit: ");
  Serial.println(selectedDigit);
  displaySerial.write('d');
  displaySerial.print(digit);
}

uint8_t getSelectedDigit() { return selectedDigit; }

void updateDisplay() {
  displaySerial.write('v');

  if (getDisplayMode() == DISPLAY_MODE_SET) {
    displaySerial.print(getValueDAC());
  } else {
    // displaySerial.print(getCurrentValue());
    if (getOutputState()) {
      uint16_t voltage = uint16_t(getCurrentValue() * (3.3 / 4095.0) * 1000);
      displaySerial.print(voltage);
    } else {
      //displaySerial.print(0);
      displaySerial.print(getValueDAC());
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
  displaySerial.write('E');
  displaySerial.print(error);
}

void displayAlarmClear() { displaySerial.write('e'); }

void serialDisplay() {

  // Serial.print("      Current: ");
  // Serial.print(getCurrentValue());
  // float voltage = getCurrentValue() * (3.3 / 4095.0);
  // Serial.print("      Current: ");
  // Serial.print(voltage, 3);
  // Serial.println();
}
