#include "cooling.h"

/******** Fan alarm currently disabled ********/

// For debugging
bool rampDir = false;                // Ramp up or down
bool fanControlMode = FAN_CTRL_AUTO; // Auto or manual
void testPWM();

// Fans
bool fanEnabled = false;
uint16_t fanPWM = 0; // fanMinPWM;
volatile uint32_t tachPulseCount = 0;
volatile uint16_t fanRPM = 0;

uint16_t total = 0; // NTC running total

void configureCooling() {

  pinMode(pinFanEnable, OUTPUT);
  pinMode(pinFanTach, INPUT);

  digitalWrite(pinFanEnable, LOW);

  attachInterrupt(digitalPinToInterrupt(pinFanTach), fanTachInterruptHandler,
                  FALLING);

  setFanPWM(0); // Set fan to minimum to start with
}

void handleCooling() {

  uint32_t currentMillis = millis();
  static uint32_t previousMillis = 0;

  if ((uint32_t)(currentMillis - previousMillis) >= COOLING_INTERVAL) {

    previousMillis = currentMillis;

    if (getOutputState() && getNTC() > OVERTEMP_THRESHOLD && !getAlarmFlag()) {
      setAlarm(ALARM_OVER_TEMP);
    }

    else if (fanEnabled) {

      if (!getOutputState() && getNTC() < FAN_OFF_THRESHOLD) {
        Serial.println("Output off and cooled down");
        setFanState(false);
        return;
      }

      fanRPM = tachPulseCount * (60 / TACH_PULSE_PER_ROTATION);
      tachPulseCount = 0;

      // Fan Tach alarm
      if (fanRPM == 0 && !getAlarmFlag()) {
        setAlarm(ALARM_FAN_FAIL);
      }

      Serial.print("RPM: ");
      Serial.println(fanRPM);
      Serial.println();
    }
    // Start fan
    else if (getNTC() >= NTC_FAN_THRESHOLD && !fanEnabled && getOutputState()) {
      setFanState(true);
    }
  }
}

// ******************************* Fan *******************************

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

void setFanPWM(uint16_t value) {
  analogWrite(pinFanPWM, value);
  Serial.print("New PWM: ");
  Serial.println(value);
}

void fanTachInterruptHandler() { tachPulseCount++; }

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