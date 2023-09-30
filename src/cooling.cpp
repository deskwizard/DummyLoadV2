#include "cooling.h"

/******** Fan alarm currently disabled ********/

// For debugging
bool rampDir = false;                // Ramp up or down
bool fanControlMode = FAN_CTRL_AUTO; // Auto or manual

// Fans
bool fanEnabled = false;
uint16_t fanPWM = fanMinPWM;
volatile uint32_t tachPulseCount = 0;
volatile uint16_t fanRPM = 0;

uint16_t total = 0; // NTC running total

void configureCooling() {

  pinMode(pinFanEnable, OUTPUT);
  pinMode(pinFanTach, INPUT);
  pinMode(pinAlarmLED, OUTPUT);

  digitalWrite(pinFanEnable, LOW);
}

void handleCooling() {

  static bool ledState = false;

  uint32_t currentMillis = millis();
  static uint32_t previousMillis = 0;

  if ((uint32_t)(currentMillis - previousMillis) >= 1000) {

    if (getAlarmFlag()) {
      ledState = !ledState;
      digitalWrite(pinAlarmLED, ledState);
    }

    fanRPM = tachPulseCount * 60;
    tachPulseCount = 0;

    // Fan tach alarm
    if (fanEnabled && fanRPM == 0 && !getAlarmFlag()) {
      // TODO: bypass temporarily
      // setAlarm(ALARM_FAN_FAIL);
    }

    if (getOutputState() && getNTC() > MAX_NTC && !getAlarmFlag()) {
      setAlarm(ALARM_OVER_TEMP);
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

// ******************************* NTC *******************************

void readNTC() {

  static uint8_t readingIndex = 0; // the index of the current reading
  static uint16_t readings[NTC_READ_COUNT] = {0};

  total = total - readings[readingIndex];      // subtract the last reading
  readings[readingIndex] = analogRead(pinNTC); // read from the sensor
  total = total + readings[readingIndex];      // add the reading to the total
  readingIndex++; // advance to the next position in the array:

  // if we're at the end of the array, wrap around to the beginning
  if (readingIndex >= NTC_READ_COUNT) {
    readingIndex = 0;
  }
}

uint16_t getNTC() { return total / NTC_READ_COUNT; }

void printTemperature() {

  uint16_t Vo = total / NTC_READ_COUNT; // the average

  float R2 = R1 * (1023.0 / (float)Vo - 1.0);
  float logR2 = log(R2);
  float T = (1.0 / (C1 + C2 * logR2 + C3 * logR2 * logR2 * logR2));
  T = T - 273.15; // result in C

  Serial.print("Vo: ");
  Serial.print(Vo);
  Serial.print("  T: ");
  Serial.print(T);
  Serial.println("°C");
}