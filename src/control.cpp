#include "control.h"
#include "MCP4922.h"
#include "SPI.h"
#include "cooling.h"
#include "display.h"

MCP4922 DAC;

uint16_t outputCurrent = 0;
bool outputEnabled = false;
bool outputRange = LOW;
bool alarmTriggeredFlag = false;
float VREF = 4.098;

void setVref(float value) { VREF = value; }

void configureControls() {

  DAC.begin(DAC_CS); // Custom CS pin
  DAC.shutdown(false);

  pinMode(pinEnableRelay, OUTPUT);
  pinMode(pinRangeRelay, OUTPUT);
  digitalWrite(pinEnableRelay, LOW);
  digitalWrite(pinRangeRelay, LOW);
}

void setOutputState(bool state) {
  digitalWrite(pinEnableRelay, state);
  outputEnabled = state;
  Serial.print("Output state changed to: ");
  Serial.println(state);
}

bool getOutputState() { return outputEnabled; }

void setOutputRange(bool state) {

  outputRange = state;

  Serial.print("Output Range changed to: ");

  if (outputRange == RANGE_HIGH) {

    // Switching from low range to high range
    Serial.println("High");

  } else {
    // switching from high range to low range
    Serial.println("Low");

    if (outputCurrent > getMaxCurrent()) {
      outputCurrent = getMaxCurrent();
    }
  }

  // here
  setCurrent(outputCurrent);

  digitalWrite(pinRangeRelay, state);
}

bool getOutputRange() { return outputRange; }

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

  if (outputRange == RANGE_LOW) {
    outputCode = outputCode * 10;
  }

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

uint16_t getMaxCurrent() {
  if (outputRange == RANGE_LOW) {
    return MAX_CURRENT / 10;
  } else {
    return MAX_CURRENT;
  }
}

void testDAC() {

  static uint16_t out_a = 0;
  static uint16_t out_b = 0;

  unsigned long currentMillis = millis(); // Get snapshot of time
  static unsigned long previousMillis =
      0; // Tracks the time since last event fired

  if ((unsigned long)(currentMillis - previousMillis) >= 250) {

    if (out_a < 4095) {
      out_a = out_a + 10;
    } else {
      out_a = 0;
    }

    DAC.setDAC(CHAN_A, out_a);

    Serial.print("A: ");
    Serial.print(out_a);
    Serial.print(" ");
    Serial.println(float(4096 / (VREF * 1000)) + 1 * out_a);

    if (out_b < 4095) {
      out_b = out_b + 25;
    } else {
      out_b = 0;
    }

    DAC.setDAC(CHAN_B, out_b);
    Serial.print("B: ");
    Serial.println(out_b);

    Serial.println();

    previousMillis = currentMillis;
  }
}
