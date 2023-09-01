#include "input.h"
#include "control.h"

HardwareTimer inputTimer(HW_TIMER_INPUT);

volatile bool enableSwitchState;
bool enableSwitchLastState;

volatile bool rangeSwitchState;
bool rangeSwitchLastState;

volatile uint8_t currentEncoderPosition = 0;
uint8_t lastEncoderPosition = 0;
volatile bool encoderSwitchState;
bool encoderSwitchLastState;

extern uint16_t outputValueDAC;

void configureInputs() {

  pinMode(pinEnableSwitch, INPUT_PULLUP);

  pinMode(pinRangeSwitch, INPUT_PULLUP);

  pinMode(pinEncoderSwitch, INPUT_PULLUP);
  pinMode(pinEncoderA, INPUT_PULLUP);
  pinMode(pinEncoderB, INPUT_PULLUP);

  // Preload current switch state
  enableSwitchState = digitalRead(pinEnableSwitch);
  enableSwitchLastState = enableSwitchState;

  rangeSwitchState = digitalRead(pinRangeSwitch);
  enableSwitchLastState = rangeSwitchState;

  encoderSwitchState = digitalRead(pinEncoderSwitch);
  rangeSwitchLastState = encoderSwitchState;

  // Preload encoder state

  // Configure timer (math is all wrong, freq. is correct-ish...)
  inputTimer.setPrescaleFactor(
      2564); // Set prescaler to 2564 => fanTachTimer frequency = 168MHz/2564 =
             // 65522 Hz (from prediv'd by 1 clocksource of 168 MHz)

  // Set overflow to 163 => fanTachTimer frequency = 65522 Hz / 163 = 10ms(?)
  inputTimer.setOverflow(163);

  inputTimer.attachInterrupt(inputTimerInterrupt);
  inputTimer.refresh(); // Make register changes take effect
  inputTimer.resume();  // Start
}

void handleInputs() {

  if (currentEncoderPosition != lastEncoderPosition) {

    if ((currentEncoderPosition == 3 && lastEncoderPosition == 1)) {
      Serial.println("Encoder +");
      if (outputValueDAC >= maxDAC) {
        outputValueDAC = maxDAC;
      } else {
        outputValueDAC = outputValueDAC + ENCODER_STEP;
      }

    } else if ((currentEncoderPosition == 2 && lastEncoderPosition == 0)) {
      Serial.print("Encoder -   ");
      if (outputValueDAC == 0) {
        outputValueDAC = 0;
      } else {
        outputValueDAC = outputValueDAC - ENCODER_STEP;
      }
    }

    setDAC(outputValueDAC);
    lastEncoderPosition = currentEncoderPosition;
  }

  if (encoderSwitchState != encoderSwitchLastState) {
    Serial.print("Encoder switch: ");
    Serial.println(encoderSwitchState);
    encoderSwitchLastState = encoderSwitchState;

    if (encoderSwitchState == EN_SW_DOWN) {
      if (getAlarmFlag()) {
        clearAlarm();
      }
    }
  }

  if (enableSwitchState != enableSwitchLastState) {
    Serial.print("Enable switch: ");
    Serial.println(enableSwitchState);
    enableSwitchLastState = enableSwitchState;

    if (enableSwitchState == EN_SW_DOWN) {
      if (getOutputState()) {
        setOutputState(OUTPUT_OFF);
      } else {
        setOutputState(OUTPUT_ON);
      }
    }
  }

  if (rangeSwitchState != rangeSwitchLastState) {
    Serial.print("Range switch: ");
    Serial.println(rangeSwitchState);
    rangeSwitchLastState = rangeSwitchState;
  }

}

void inputTimerInterrupt() {

  currentEncoderPosition = 0;

  if (!digitalRead(pinEncoderA)) {
    currentEncoderPosition |= (1 << 1);
  }
  if (!digitalRead(pinEncoderB)) {
    currentEncoderPosition |= (1 << 0);
  }

  encoderSwitchState = digitalRead(pinEncoderSwitch);
  enableSwitchState = digitalRead(pinEnableSwitch);
  rangeSwitchState = digitalRead(pinRangeSwitch);
}