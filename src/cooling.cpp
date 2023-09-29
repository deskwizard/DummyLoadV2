#include "cooling.h"

// For debugging
bool rampDir = false;                // Ramp up or down
bool fanControlMode = FAN_CTRL_AUTO; // Auto or manual

bool fanEnabled = false;
uint16_t fanPWM = fanMinPWM;
volatile uint32_t tachPulseCount = 0;
volatile uint16_t fanRPM = 0;

void configureCooling() {

  pinMode(pinFanEnable, OUTPUT);
  pinMode(pinFanTach, INPUT);
  pinMode(pinAlarmLED, OUTPUT);

  digitalWrite(pinFanEnable, LOW);
}

// ******************************* Fans *******************************

void setFanMode(bool mode) {
  fanControlMode = mode;
  Serial.print("Fan mode: ");
  Serial.println(fanControlMode);
}

void setFanState(bool state) {
  fanEnabled = state;
  Serial.print("Fan state: ");
  Serial.println(fanEnabled);
  digitalWrite(pinFanEnable, fanEnabled);
}

bool getFanState() { return fanEnabled; }

uint16_t getFanRPM() { return fanRPM; }

uint16_t getFanPWM() { return fanPWM; }

void oneSecondTimerInterrupt() {

  static volatile bool ledState = false;

  if (getAlarmFlag()) {
    ledState = !ledState;
    digitalWrite(pinAlarmLED, ledState);
  }

  fanRPM = tachPulseCount * 60;
  tachPulseCount = 0;

  if (fanEnabled && fanRPM == 0 && !getAlarmFlag()) {
    // TODO: bypass temporarily
    // setAlarm(ALARM_FAN_FAIL);
  }
}

void fanTachInterruptHandler() { tachPulseCount++; }