#include "control.h"
#include "cooling.h"
#include "display.h"

uint16_t outputCurrent = 0;
bool outputEnabled = false;
bool alarmTriggeredFlag = false;

/*
// Reuse for NTC ?
const uint8_t currentDataPointCount;
uint16_t currentReadings[currentDataPointCount] = {0};
uint8_t currentReadIndex = 0;
uint64_t currentRunningTotal = 0;
*/

void configureControls() {

  Wire.begin();
  Wire.setClock(4000000);
  setDAC(0);

  pinMode(pinOutputEnableRelay, OUTPUT);
  digitalWrite(pinOutputEnableRelay, LOW);
}

void setOutputState(bool state) {
  digitalWrite(pinOutputEnableRelay, state);
  outputEnabled = state;
  setFanState(state);
  Serial.print("Output state changed to: ");
  Serial.println(state);
}

bool getOutputState() { return outputEnabled; }

void setAlarm(uint8_t alarmType) {
  alarmTriggeredFlag = true;

  // No matter what, turn output off, set fan to max speed
  setOutputState(OUTPUT_OFF);

  displayAlarmSet(alarmType);

  Serial.print("********** ALARM: ");

  if (alarmType == ALARM_FAN_FAIL) {
    Serial.print("Fan Failure");
  } else if (alarmType == ALARM_OVER_TEMP) {
    Serial.print("Over Temperature");
  } else if (alarmType == ALARM_OVER_CURRENT) {
    Serial.print("Over Current");
  }
  Serial.println(" **********");
}

void clearAlarm() {
  alarmTriggeredFlag = false;
  displayAlarmClear();
  digitalWrite(pinAlarmLED, LOW);
  Serial.println("********** Alarm(s) Cleared **********");
}

bool getAlarmFlag() { return alarmTriggeredFlag; }

void setCurrent(uint16_t current) {

  outputCurrent = current;

  // I could make that calculation in one line, but it'll take me
  // ages to figure it out if I ever have to look at this again.
  float value = (float(current) + 0.5) / 1000.0;
  uint16_t outputCode = uint16_t((value / VREF) * 4095.0);
  setDAC(outputCode);

  displayValue(outputCurrent);

  Serial.print("Output current: ");
  Serial.print(value, 3);
  Serial.print(" A");
  Serial.println();
}

void setDAC(uint16_t output_value) {

  Wire.begin();
  Wire.beginTransmission(MCP4725_ADDR);
  Wire.write(64);                       // Update DAC command
  Wire.write(output_value >> 4);        // the 8 most significant bits...
  Wire.write((output_value & 15) << 4); // the 4 least significant bits...
  Wire.endTransmission();
  Wire.end();

  Serial.print("DAC output: ");
  Serial.print(output_value);
  Serial.println();
}

uint16_t getCurrent() { return outputCurrent; }
