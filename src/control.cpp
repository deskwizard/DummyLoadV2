#include "control.h"
#include "cooling.h"

bool alarmTriggeredFlag = false;
bool outputEnabled = false;

uint16_t outputValueDAC = 1890;

void configureControls() {

  pinMode(pinDacOutput, OUTPUT);
  pinMode(pinOutputEnable, OUTPUT);
  pinMode(pinOutputStateLED, OUTPUT);
  digitalWrite(pinOutputEnable, LOW);
  digitalWrite(pinDacOutput, LOW);
  digitalWrite(pinOutputStateLED, LOW);

  setDAC(outputValueDAC);
  Serial.print("DAC output: ");
  Serial.println(outputValueDAC);
}

void setOutputState(bool state) {
  digitalWrite(pinOutputEnable, state);
  digitalWrite(pinOutputStateLED, state);
  outputEnabled = state;
}

bool getOutputState() { return outputEnabled; }

void setAlarm(uint8_t alarmType) {
  alarmTriggeredFlag = true;

  // No matter what, turn output off, set fan to max speed
  setOutputState(OUTPUT_OFF);
  setFanSpeed(fanMaxPWM);
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
  digitalWrite(pinDebugLED, LOW);
  Serial.println("********** Alarm(s) Cleared **********");
}

bool getAlarmFlag() { return alarmTriggeredFlag; }

void setDAC(uint16_t value) {

  if (value >= maxDAC) {
    value = maxDAC;
  }
  outputValueDAC = value;
  analogWrite(pinDacOutput, value);
  Serial.print("DAC output: ");
  Serial.println(outputValueDAC);
}