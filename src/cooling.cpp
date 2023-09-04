#include "cooling.h"

HardwareTimer fanTachTimer(HW_TIMER_COOLING);

uint16_t tempReadings[dataPointCount] = {0};
uint8_t tempReadIndex = 0;
uint64_t tempRunningTotal = 0;

// For debugging
bool rampDir = false;                // Ramp up or down
bool fanControlMode = FAN_CTRL_AUTO; // Auto or manual

bool fanEnabled = false;
uint16_t fanPWM = fanMinPWM;
volatile uint32_t tachPulseCount = 0;
volatile uint16_t fanRPM = 0;

void configureCooling() {

  pinMode(pinFanEnable, OUTPUT);
  pinMode(pinFanPWM, OUTPUT);
  pinMode(pinFanTach, INPUT);

  analogWrite(pinFanPWM, fanPWM);
  digitalWrite(pinFanEnable, LOW);

  attachInterrupt(digitalPinToInterrupt(pinFanTach), fanTachInterruptHandler,
                  RISING);

  // Configure fanTachTimer (math is all wrong, freq. is correct-ish...)
  fanTachTimer.setPrescaleFactor(
      2564); // Set prescaler to 2564 => fanTachTimer frequency = 168MHz/2564 =
             // 65522 Hz (from prediv'd by 1 clocksource of 168 MHz)

  // Set overflow to 16380 => fanTachTimer frequency = 65522 Hz / 16380 = 1 Hz
  fanTachTimer.setOverflow(16380);

  fanTachTimer.attachInterrupt(oneSecondTimerInterrupt);
  fanTachTimer.refresh(); // Make register changes take effect
  fanTachTimer.resume();  // Start
}

void handleCooling() {

  uint32_t currentMillis = millis();
  static uint32_t previousMillis = 0;

  if ((uint32_t)(currentMillis - previousMillis) >= tempReadInterval) {

    readNTC();

    if (fanControlMode) {
      // do nothing... I'm sure there's a smarter way, tbd.
    } else if (!fanEnabled &&
               (getNTCValue() > (fanEnableThreshold + fanThresholdDeadband))) {
      setFanState(true);
    } else if (fanEnabled && (getNTCValue() < (fanEnableThreshold -
                                               (fanThresholdDeadband * 2)))) {

      setFanState(false);
    }

    previousMillis = currentMillis;
  }

  /*
  // PWM Auto ramp debug
    uint32_t currentMillis2 = millis();
    static uint32_t previousMillis2 = 0;
    if ((uint32_t)(currentMillis2 - previousMillis2) >= 10000) {

      if (fanControlMode) {
        if (rampDir) {
          if (fanPWM >= 4000) {
            fanPWM = 0;
          } else {
            fanPWM = fanPWM + 200;
          }
          setFanSpeed(fanPWM);
        } else {
          if (fanPWM == 0) {
            fanPWM = 4000;
          } else {
            fanPWM = fanPWM - 200;
          }
          setFanSpeed(fanPWM);
        }
      }
      previousMillis2 = currentMillis2;
    } */
}

// ******************************* NTC *******************************

void readNTC() {

  // Subtract the last reading
  tempRunningTotal = tempRunningTotal - tempReadings[tempReadIndex];

  // Read from the sensor
  tempReadings[tempReadIndex] = analogRead(pinNTC);

  // Add the reading to the tempRunningTotal
  tempRunningTotal = tempRunningTotal + tempReadings[tempReadIndex];

  // Advance to the next position in the array
  tempReadIndex = tempReadIndex + 1;

  // If we're at the end of the array, wrap around to the beginning
  if (tempReadIndex >= dataPointCount) {
    tempReadIndex = 0;
  }
}

uint16_t getNTCValue() { return tempRunningTotal / dataPointCount; }

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
  setFanSpeed(fanMinPWM);
}

void setFanSpeed(uint16_t value) {
  // Serial.print("New PWM: ");
  // Serial.println(value);
  fanPWM = value;
  analogWrite(pinFanPWM, fanPWM);
  fanTachTimer.setCount(0);
  tachPulseCount = 0;
}

bool getFanState() { return fanEnabled; }

uint16_t getFanRPM() { return fanRPM; }

uint16_t getFanPWM() { return fanPWM; }

void oneSecondTimerInterrupt() {

  static volatile bool ledState = false;

  if (getAlarmFlag()) {
    ledState = !ledState;
    digitalWrite(pinDebugLED, ledState);
  }

  fanRPM = tachPulseCount * 60;
  tachPulseCount = 0;

  if (fanEnabled && fanRPM == 0 && !getAlarmFlag()) {
    setAlarm(ALARM_FAN_FAIL);
  }

  if (getOutputState() && getNTCValue() >= NTC_OT_THRESHOLD) {
    setAlarm(ALARM_OVER_TEMP);
  }

  uint16_t value = tempRunningTotal / dataPointCount;
  if (fanEnabled && (value >= fanEnableThreshold) && !getAlarmFlag()) {
    int16_t mapped =
        map(value, fanEnableThreshold, 2800, fanMinPWM, fanMaxPWM);
    // Serial.print("Mapped: ");
    // Serial.println(mapped);

    if (mapped < fanMaxPWM) {
      mapped = fanMaxPWM;
    }

    setFanSpeed(mapped);
  }
}

void fanTachInterruptHandler() { tachPulseCount++; }