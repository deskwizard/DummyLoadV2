/*
  Notes:
      - The NTC read is in the input timer interrupt for now


  Ideas:

    - use DPST switch for range, one side to signal the mcu, one side
      for the actual range (and LED? Would save us a pin)
    - Change fan tach routine to save last pulse and trigger if > timeout
    - OC alarm if we get there

  Todo:
      - Immediately display the underscore when we change digit set
          - now it does, but it messes up with the blinking since
            it's the same millis() stuff.
      - Inhibit output enable until cooled down on overtemp
      - Move timer stuff to a separate file
      - Fan PWM *
      - Fan Tach *


  To fix:  *** NOT UP TO DATE, IGNORE FOR THE TIME BEING ***

  - Fan tach reading is fine at RPM min/max but not at intermediate speed ??
  - Mapping from NTC value top PWM value feels dodgy
          -> It underflows before the specified value ("Fixed")
          -> It's mapping from 2550 or w/e instead of 2600 for some reason??
          -> We don't need to update the fan speed every second do we...
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

void testDAC();
void handleSerialInput();
void flashDebugLED();
void i2cScan();

extern bool rampDir; // Debug

void setup() {

  Wire.begin();

  Serial.setTimeout(200);
  Serial.begin(115200);
  Serial.println("Hello");
  Serial.println();

  configureControls();
  configureCooling();
  configureInputs();
  configureDisplay();
}

void loop() {

  handleCooling();
  handleInputs();
  handleDisplay();
  handleSerialInput();
}

void testDAC() {

  uint32_t currentMillis = millis();
  static uint32_t previousMillis = 0;
  static uint16_t value = 0;

  if ((uint32_t)(currentMillis - previousMillis) >= 2000) {

    if (value < 1000) {
      value = value + 100;
      setCurrent(value);
    } else {
      value = 0;
      setCurrent(value);
    }

    previousMillis = currentMillis;
  }
}

void handleSerialInput() {

  if (Serial.available()) {

    int8_t c = Serial.read();
    uint32_t readValue;

    switch (c) {

    case 'p':
      readValue = Serial.parseInt();
      setFanPWM(readValue);
      break;
    case 'd':
      readValue = Serial.parseInt();
      Serial.print("d: ");
      Serial.println(readValue);
      setDAC(readValue);
      break;

    case 'f':
      setFanMode(FAN_CTRL_MAN);
      setFanState(false);
      break;
    case 'F':
      setFanMode(FAN_CTRL_MAN);
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

    case 'c':
      readValue = Serial.parseInt();
      Serial.print("Current: ");
      Serial.println(readValue);
      setCurrent(readValue);
      break;

    case 's':
      i2cScan();
      break;

    case 'q':
      setAlarm(0);
      break;

    case 'w':
      clearAlarm();
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
    digitalWrite(pinAlarmLED, ledState);

    previousMillis = currentMillis;
  }
}

void i2cScan() {
  uint8_t error, address;
  int16_t nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++) {

    // The return value of the Write.endTransmission
    // to see if a device did acknowledge to the address.

    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknow error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}