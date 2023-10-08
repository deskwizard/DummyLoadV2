#include "control.h"
#include "MCP4922.h"
#include "cooling.h"
#include "display.h"

MCP4922 DAC;

uint16_t outputCurrent = 0;
bool outputEnabled = false;
bool alarmTriggeredFlag = false;
float VREF = 4.096;

void setVref(float value) { VREF = value; }

void configureControls() {

  DAC.begin(A3); // Custom CS pin
  setDAC(0);
  DAC.shutdown(false);

  pinMode(pinEnableRelay, OUTPUT);
  pinMode(pinRangeRelay, OUTPUT);
  digitalWrite(pinEnableRelay, LOW);
  digitalWrite(pinRangeRelay, LOW);
}

void setOutputState(bool state) {
  digitalWrite(pinEnableRelay, state);
  outputEnabled = state;
  // setFanState(state);
  Serial.print("Output state changed to: ");
  Serial.println(state);
}

bool getOutputState() { return outputEnabled; }

void setAlarm(uint8_t alarmType) {
  alarmTriggeredFlag = true;

  // No matter what, turn output off, set fan to max speed
  setFanPWM(255); // Shouldn't happen but just in case
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
  setDisplayMode(DISPLAY_MODE_VALUE);
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

  DAC.setDAC(CHAN_A, output_value);

  Serial.print("DAC output: ");
  Serial.print(output_value);
  Serial.println();
}

uint16_t getCurrent() { return outputCurrent; }
