#include "display.h"
#include "cooling.h"

void configureDisplay() {}

void handleDisplay() {

  uint32_t currentMillis = millis();
  static uint32_t previousMillis = 0;

  if ((uint32_t)(currentMillis - previousMillis) >= DISPLAY_UPDATE_TIME) {
    serialDisplay();
    previousMillis = currentMillis;
  }
}

void serialDisplay() {

  Serial.print("NTC: ");
  Serial.print(getNTCValue());

  if (getFanState()) {
    Serial.print("     PWM: ");
    Serial.print(getFanPWM());
  }

  Serial.println();

  // Serial.print("RPM: ");
  // Serial.println(getFanRPM());

  Serial.println();
}