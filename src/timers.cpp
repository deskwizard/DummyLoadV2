#include "timers.h"
#include "cooling.h"
#include "input.h"

bool timerNeedHandling = false;

void configureTimer() {

  // TIMER 1 for interrupt frequency 1000 Hz:

  cli();      // Disable interrupts
  TCCR1A = 0; // Set entire TCCR1A register to 0
  TCCR1B = 0; // Same for TCCR1B
  TCNT1 = 0;  // Initialize counter value to 0

  // Set compare match register for 1000 Hz increments
  OCR1A = 15999; // = 16000000 / (1 * 1000) - 1 (must be <65536)

  // Turn on CTC mode
  TCCR1B |= (1 << WGM12);

  // Set CS12, CS11 and CS10 bits for 1 prescaler
  TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);

  // Enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei(); // Enable interrupts
}

ISR(TIMER1_COMPA_vect) { timerNeedHandling = true; }

void handleTimers() {

  if (timerNeedHandling == true) {

    readVoltage();
    readNTC();

    currentEncoderPosition = 0;

    if (!digitalRead(pinEncoderA)) {
      currentEncoderPosition |= (1 << 1);
    }
    if (!digitalRead(pinEncoderB)) {
      currentEncoderPosition |= (1 << 0);
    }

    encoderSwitchState = digitalRead(pinEncoderSwitch);
    enableSwitchState = digitalRead(pinOutputEnableSwitch);
    rangeSwitchState = digitalRead(pinRangeSwitch);

    timerNeedHandling = false;
  }
}