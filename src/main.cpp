/*
  Notes:

    - 260-ish mA current draw full load


  Ideas:
    
    - Inhibit output enable until cooled down on overtemp
    - OC alarm?


  Todo:

*/

#include "control.h"
#include "cooling.h"
#include "defines.h"
#include "display.h"
#include "input.h"
#include "timers.h"

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
  configureTimer();
  
}

void loop() {

  handleCooling();
  handleInputs();
  handleDisplay();
  handleSerialInput();
  handleTimers();
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
