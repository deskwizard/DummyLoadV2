/*
  Notes:
    - use DPST switch for range, one side to signal the mcu, one side
      for the actual range (and LED? Would save us a pin)
    - Should we move the output status LED to the free contacts on the
      relay? That would save another pin...

  Todo:
  - Some way to make the DAC outputs "even"
  - Live Current ADC reading (will need hardware change)
  - Fan tach reading is fine at RPM min/max but not at intermediate speed
  - setDAC is called 3 times per encoder step
  - Mapping from NTC value top PWM value feels dodgy (starts 2000 for reasons)
          -> It's mapping from 2550 or w/e instead of 2600 for some reason??
  - Output enable switch debounce (switch dependant...)

*/

#include "control.h"
#include "cooling.h"
#include "defines.h"
#include "display.h"
#include "input.h"
#include <Wire.h>

void flashDebugLED();
void handleSerialInput();
void scanI2C();

extern bool rampDir;

void setup() {

  Serial.setRx(serialRX);
  Serial.setTx(serialTX);
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

  Wire.setSDA(pinSDA);
  Wire.setSCL(pinSCL);
  Wire.begin();
  // scanI2C();
}

void loop() {

  handleCooling();
  handleInputs();
  handleDisplay();
  handleSerialInput();

  flashDebugLED();
}

void scanI2C() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
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

void handleSerialInput() {

  if (Serial.available()) {

    int8_t c = Serial.read();
    uint32_t readValue;

    switch (c) {
    case 'd':
      readValue = Serial.parseInt();
      Serial.print("d: ");
      Serial.println(readValue);
      break;

      // TODO: Those two cases feel fishy and need a cleanup
    case '+':
      if (getFanPWM() == 0) {
        setFanSpeed(fanMinPWM);
      } else {
        setFanSpeed(getFanPWM() - 200);
      }
      break;

    case '-':
      if (getFanPWM() >= 4000) {
        setFanSpeed(fanMaxPWM);
      } else {
        setFanSpeed(getFanPWM() + 200);
      }
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

    // ledState = !ledState;
    // digitalWrite(pinDebugLED, ledState);

    previousMillis = currentMillis;
  }
}