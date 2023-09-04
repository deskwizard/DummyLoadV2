#include "control.h"
#include "cooling.h"
#include "display.h"

HardwareTimer controlTimer(HW_TIMER_CONTROL);

#define dataPointCount 50

bool alarmTriggeredFlag = false;
bool outputEnabled = false;

uint16_t currentReadings[dataPointCount] = {0};
uint8_t currentReadIndex = 0;
uint64_t currentRunningTotal = 0;

uint16_t outputValueDAC = 1234;

void controlTimerInterrupt() { readCurrent(); }

void configureControls() {

  pinMode(pinDacOutput, OUTPUT);
  pinMode(pinOutputEnable, OUTPUT);
  pinMode(pinCurrentADC, INPUT_ANALOG);
  digitalWrite(pinOutputEnable, LOW);
  digitalWrite(pinDacOutput, LOW);


  setDAC(outputValueDAC);

  // Configure controlTimer (math is all wrong, freq. is correct-ish...)
  controlTimer.setPrescaleFactor(
      2564); // Set prescaler to 2564 => controlTimer frequency = 168MHz/2564 =
             // 65522 Hz (from prediv'd by 1 clocksource of 168 MHz)

  // Set overflow to 16380 => controlTimer frequency = 65522 Hz / 16380 = 1 Hz
  controlTimer.setOverflow(163);

  controlTimer.attachInterrupt(controlTimerInterrupt);
  controlTimer.refresh(); // Make register changes take effect
  controlTimer.resume();  // Start

  Serial.print("DAC output: ");
  Serial.println(outputValueDAC);
}

void setOutputState(bool state) {
  digitalWrite(pinOutputEnable, state);
  outputEnabled = state;
}

bool getOutputState() { return outputEnabled; }

void setAlarm(uint8_t alarmType) {
  alarmTriggeredFlag = true;

  // No matter what, turn output off, set fan to max speed
  setOutputState(OUTPUT_OFF);
  setFanSpeed(fanMaxPWM);

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

uint16_t getValueDAC() { return outputValueDAC; }

void readCurrent() {

  // Subtract the last reading
  currentRunningTotal = currentRunningTotal - currentReadings[currentReadIndex];

  // Read from the sensor
  currentReadings[currentReadIndex] = analogRead(pinCurrentADC);

  // Add the reading to the tempRunningTotal
  currentRunningTotal = currentRunningTotal + currentReadings[currentReadIndex];

  // Advance to the next position in the array
  currentReadIndex = currentReadIndex + 1;

  // If we're at the end of the array, wrap around to the beginning
  if (currentReadIndex >= dataPointCount) {
    currentReadIndex = 0;
  }
}
uint16_t getCurrentValue() { return currentRunningTotal / dataPointCount; }
//uint16_t getCurrentValue() { return analogRead(pinCurrentADC); }
