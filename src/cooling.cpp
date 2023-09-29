#include "cooling.h"

/******** Fan alarmed currently disabled ********/

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

void handleCooling() {

  // Alarm LED flash
  uint32_t currentMillis = millis();
  static uint32_t previousMillis = 0;
  static bool ledState = false;

  if ((uint32_t)(currentMillis - previousMillis) >= 1000) {

  if (getAlarmFlag()) {
    ledState = !ledState;
    digitalWrite(pinAlarmLED, ledState);
  }

  fanRPM = tachPulseCount * 60;
  tachPulseCount = 0;

  if (fanEnabled && fanRPM == 0 && !getAlarmFlag()) {
    // TODO: bypass temporarily
    //setAlarm(ALARM_FAN_FAIL);
  }
    previousMillis = currentMillis;
  }
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

/*
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
    setAlarm(ALARM_FAN_FAIL);
  }
}
*/
// void fanTachInterruptHandler() { tachPulseCount++; }