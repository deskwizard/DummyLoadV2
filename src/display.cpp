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

void handleDisplay() {

  uint32_t currentMillis = millis();
  static uint32_t previousMillis = 0;

  if ((uint32_t)(currentMillis - previousMillis) >= DISPLAY_UPDATE_TIME) {
    serialDisplay();
    previousMillis = currentMillis;
  }
}

void serialDisplay() {

  // Serial.print("NTC: ");
  // Serial.print(getNTCValue());

  displaySerial.write('v');
  displaySerial.print(getNTCValue());

  if (getFanState()) {
    Serial.print("     PWM: ");
    Serial.print(getFanPWM());
  }

  // Serial.println();
  // Serial.print("RPM: ");
  // Serial.println(getFanRPM());
  // Serial.println();

}

void displayAlarmSet(uint8_t error) {
  displaySerial.write('E');
  displaySerial.print(error);
}

void displayAlarmClear() {
  displaySerial.write('e');
}