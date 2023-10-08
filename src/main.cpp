/*
  Notes:
      - The NTC read is in the input timer interrupt for now


  Ideas:

    - Change fan tach routine to save last pulse and trigger if > timeout
    (- OC alarm if we get there)

  Todo:
      - Immediately display the underscore when we change digit set
          - now it does, but it messes up with the blinking since
            it's the same millis() stuff.
      - Inhibit output enable until cooled down on overtemp
      - Move timer stuff to a separate file
      - Fan PWM *
      - Fan Tach *
      - Timeout adjustement mode after X seconds
*/

#include "control.h"
#include "cooling.h"
#include "defines.h"
#include "display.h"
#include "input.h"

void testDAC();
void handleSerialInput();
void flashDebugLED();

extern bool rampDir; // Debug

void setup() {

  Serial.setTimeout(200);
  Serial.begin(115200);
  Serial.println("Hello");
  Serial.println();

  configureDisplay();
  configureControls();
  configureCooling();
  configureInputs();

}

void loop() {

  handleCooling();
  handleInputs();
  handleDisplay();
  handleSerialInput();
}

void testDAC() {

  uint32_t currentMillis = millis();
  static uint32_t previousMillis = 0;
  static uint16_t value = 0;

  if ((uint32_t)(currentMillis - previousMillis) >= 2000) {

    if (value < 1000) {
      value = value + 100;
      setCurrent(value);
    } else {
      value = 0;
      setCurrent(value);
    }

    previousMillis = currentMillis;
  }
}

void handleSerialInput() {

  if (Serial.available()) {

    int8_t c = Serial.read();
    uint32_t readValue;

    switch (c) {

    case 'p':
      readValue = Serial.parseInt();
      setFanPWM(readValue);
      break;
    case 'd':
      readValue = Serial.parseInt();
      Serial.print("d: ");
      Serial.println(readValue);
      setDAC(readValue);
      break;

    case 'f':
      setFanMode(FAN_CTRL_MAN);
      setFanState(false);
      break;
    case 'F':
      setFanMode(FAN_CTRL_MAN);
      setFanState(true);
      break;
    case 'a':
      setFanMode(FAN_CTRL_AUTO);
      Serial.println("Fan auto");
      break;
    case 'm':
      setFanMode(FAN_CTRL_MAN);
      Serial.println("Fan manual");
      break;

    case 'i':
      rampDir = !rampDir;
      Serial.println("Fan spin ramp direction change");
      break;

    case 'c':
      readValue = Serial.parseInt();
      Serial.print("Current: ");
      Serial.println(readValue);
      setCurrent(readValue);
      break;

    case 'v':
      readValue = Serial.parseInt();
      Serial.print("vref: ");
      Serial.println(readValue);
      setCurrent(readValue);
      break;

    case 'q':
      setAlarm(0);
      break;

    case 'w':
      clearAlarm();
      break;
    }
  }
} /*
 
 }
 void flashDebugLED() {

   uint32_t currentMillis = millis();
   static uint32_t previousMillis = 0;
   static bool ledState;

   if ((uint32_t)(currentMillis - previousMillis) >= debugLedFlashInterval) {

     ledState = !ledState;
     digitalWrite(pinAlarmLED, ledState);

     previousMillis = currentMillis;
   }
 }
 */
