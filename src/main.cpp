/*
  Notes:
    - use DPST switch for range, one side to signal the mcu, one side
      for the actual range (and LED? Would save us a pin)
    - Should we move the output status LED to the free contacts on the
      relay? That would save another pin...

  Todo:
  - Some way to make the DAC outputs "even"
  - Fan tach reading is fine at RPM min/max but not at intermediate speed
  - setDAC is called 3 times per encoder step
  - Mapping from NTC value top PWM value feels dodgy
          -> It underflows before the specified value ("Fixed")
          -> It's mapping from 2550 or w/e instead of 2600 for some reason??
          -> We don't need to update the fan speed every second ...
  - Output enable switch debounce (switch dependant...)
  - Fan PWM output will need evening out, do smoothing on the mapped PWM values?
  - NTC value to fan pwm map() is dodgy as f..k


*/

#include "control.h"
#include "cooling.h"
#include "defines.h"
#include "display.h"
#include "input.h"
#include <Wire.h>

void flashDebugLED();
void handleSerialInput();

extern bool rampDir;

void setup() {

  Serial.setRx(debugSerialRX);
  Serial.setTx(debugSerialTX);
  Serial.setTimeout(200);
  Serial.begin(115200);
  delay(2000);
  Serial.println("Hello");
  Serial.println();

  // Those settings need to be set first as they're
  // used for multiple different functions
  analogWriteFrequency(25000);
  analogWriteResolution(12);
  analogReadResolution(12);

  configureControls();
  configureCooling();
  configureInputs();
  configureDisplay();

  pinMode(pinDebugLED, OUTPUT);

}

void loop() {

  handleInputs();
  handleDisplay();
  handleSerialInput();

  // flashDebugLED();  // it's on the 1sec timer
}

void handleSerialInput() {

  if (Serial.available()) {

    int8_t c = Serial.read();
    uint32_t readValue;

    switch (c) {
    case 'd':
      readValue = Serial.parseInt();
      Serial.print("d: ");
      Serial.println(readValue);
      setDAC(readValue);
      break;

    case 'f':
      setFanMode(false);
      setFanState(false);
      break;
    case 'F':
      setFanMode(true);
      setFanState(true);
      break;
    case 'a':
      setFanMode(FAN_CTRL_AUTO);
      Serial.println("Fan auto");
      break;
    case 'm':
      setFanMode(FAN_CTRL_MAN);
      Serial.println("Fan manual");
      break;

    case 'i':
      rampDir = !rampDir;
      Serial.println("Fan spin ramp direction change");
      break;
    }
  }
}

void flashDebugLED() {

  uint32_t currentMillis = millis();
  static uint32_t previousMillis = 0;
  static bool ledState;

  if ((uint32_t)(currentMillis - previousMillis) >= debugLedFlashInterval) {

     ledState = !ledState;
     digitalWrite(pinDebugLED, ledState);

    previousMillis = currentMillis;
  }
}