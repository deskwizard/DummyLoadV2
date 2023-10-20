#include "input.h"
#include "control.h"
#include "display.h"
#include "timers.h"

// debug
#include "cooling.h"

volatile bool enableSwitchState;
bool enableSwitchLastState;

volatile bool rangeSwitchState;
bool rangeSwitchLastState;

volatile uint8_t currentEncoderPosition = 0;
uint8_t lastEncoderPosition = 0;
volatile bool encoderSwitchState;
bool encoderSwitchLastState;

bool lastDigit = 0;

const uint16_t encoderStep[4] = {1000, 100, 10, 1};

void configureInputs() {

  pinMode(pinOutputEnableSwitch, INPUT_PULLUP);

  pinMode(pinRangeSwitch, INPUT_PULLUP);

  pinMode(pinEncoderSwitch, INPUT_PULLUP);
  pinMode(pinEncoderA, INPUT_PULLUP);
  pinMode(pinEncoderB, INPUT_PULLUP);

  delay(100); // Wait for inputs to stabilize

  // Preload current switches states
  enableSwitchState = digitalRead(pinOutputEnableSwitch);
  enableSwitchLastState = enableSwitchState;

  rangeSwitchState = digitalRead(pinRangeSwitch);
  rangeSwitchLastState = rangeSwitchState;

  encoderSwitchState = digitalRead(pinEncoderSwitch);
  encoderSwitchLastState = encoderSwitchState;

}

void handleInputs() {

  if (currentEncoderPosition != lastEncoderPosition) {

    if ((currentEncoderPosition == 3 && lastEncoderPosition == 1)) {
      Serial.println("Encoder +");
    }
    if ((currentEncoderPosition == 2 && lastEncoderPosition == 0)) {
      Serial.println("Encoder -");
    }

    if (getDisplayMode() == DISPLAY_MODE_SET && !getAlarmFlag()) {

      resetSetModeTimeout();

      if ((currentEncoderPosition == 3 && lastEncoderPosition == 1)) {

        if (getCurrent() + encoderStep[getSelectedDigit()] >= getMaxCurrent()) {
          setCurrent(getMaxCurrent());
        } else {
          setCurrent(getCurrent() + encoderStep[getSelectedDigit()]);
        }

      } else if ((currentEncoderPosition == 2 && lastEncoderPosition == 0)) {

        int16_t tempValue = getCurrent() - encoderStep[getSelectedDigit()];

        Serial.print("get: ");
        Serial.print(getCurrent());
        Serial.print("   value: ");
        Serial.println(tempValue);

        if (tempValue < 0) {
          Serial.println("Underflow");
          setCurrent(0);
        } else {
          setCurrent(getCurrent() - encoderStep[getSelectedDigit()]);
        }
      }
    }

    lastEncoderPosition = currentEncoderPosition;
  } // encoders

  if (encoderSwitchState != encoderSwitchLastState) {

    Serial.print("Encoder switch: ");
    Serial.println(encoderSwitchState);
    encoderSwitchLastState = encoderSwitchState;

    static uint8_t counter = 3; // For "rollover" purposes

    if (encoderSwitchState == SW_DOWN) {

      if (getAlarmFlag()) {
        clearAlarm();
        return;
      }

      if (getDisplayMode() == DISPLAY_MODE_VALUE) {

        resetSetModeTimeout();

        setDisplayMode(DISPLAY_MODE_SET);
        counter = 3;
        selectSetDigit(counter);

      } else if (getDisplayMode() == DISPLAY_MODE_SET) {

        resetSetModeTimeout();

        if (getSelectedDigit() != lastDigit) {
          counter--;
          selectSetDigit(counter);
        } else {
          counter = 3;
          selectSetDigit(counter);
          // setDisplayMode(DISPLAY_MODE_VALUE);
        }
      }
    }
  }

  if (enableSwitchState != enableSwitchLastState) {
    Serial.print("Enable switch: ");
    Serial.println(enableSwitchState);
    enableSwitchLastState = enableSwitchState;

    if (enableSwitchState == SW_DOWN) {
      if (getOutputState()) {
        setOutputState(OUTPUT_OFF);
      } else if (!getOutputState() && !getAlarmFlag()) {
        setOutputState(OUTPUT_ON);
      }
    }
  }

  if (rangeSwitchState != rangeSwitchLastState) {
    rangeSwitchLastState = rangeSwitchState;

    Serial.print("Range switch: ");
    Serial.println(rangeSwitchState);
    if (rangeSwitchState == SW_DOWN) {
      setOutputRange(!getOutputRange());
      lastDigit = getOutputRange();
    }
  }
}