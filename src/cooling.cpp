#include "cooling.h"

HardwareTimer fanTachTimer(HW_TIMER_COOLING);

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
    digitalWrite(pinDebugLED, ledState);
  }

  fanRPM = tachPulseCount * 60;
  tachPulseCount = 0;

  if (fanEnabled && fanRPM == 0 && !getAlarmFlag()) {
    setAlarm(ALARM_FAN_FAIL);
  }

}

void fanTachInterruptHandler() { tachPulseCount++; }