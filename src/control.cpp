#include "control.h"
#include "cooling.h"
#include "display.h"

// HardwareTimer controlTimer(HW_TIMER_CONTROL);

// #define currentDataPointCount 50

bool alarmTriggeredFlag = false;
bool outputEnabled = false;
/*
uint16_t currentReadings[currentDataPointCount] = {0};
uint8_t currentReadIndex = 0;
uint64_t currentRunningTotal = 0;
*/
// uint16_t outputValueDAC = 0;

uint16_t outputCurrent = 0;

// void controlTimerInterrupt() { // readCurrent();
// }

void configureControls() {

  Wire.begin();
  // Wire.setClock(4000000);
  setDAC(0);

  // pinMode(pinDacOutput, OUTPUT);
  // pinMode(pinCurrentADC, INPUT_ANALOG);
  pinMode(pinOutputEnableRelay, OUTPUT);
  digitalWrite(pinOutputEnableRelay, LOW);
  // digitalWrite(pinDacOutput, LOW);

  /*
    // Configure controlTimer (math is all wrong, freq. is correct-ish...)
    controlTimer.setPrescaleFactor(
        2564); // Set prescaler to 2564 => controlTimer frequency = 168MHz/2564
    =
               // 65522 Hz (from prediv'd by 1 clocksource of 168 MHz)

    // Set overflow to 16380 => controlTimer frequency = 65522 Hz / 16380 = 1 Hz
    controlTimer.setOverflow(163);

    controlTimer.attachInterrupt(controlTimerInterrupt);
    controlTimer.refresh(); // Make register changes take effect
    controlTimer.resume();  // Start
  */
}

void setOutputState(bool state) {
  digitalWrite(pinOutputEnableRelay, state);
  outputEnabled = state;
  setFanState(state);
  Serial.print("Output state changed to: ");
  Serial.println(state);
}

bool getOutputState() { return outputEnabled; }

void setAlarm(uint8_t alarmType) {
  alarmTriggeredFlag = true;

  // No matter what, turn output off, set fan to max speed
  setOutputState(OUTPUT_OFF);

  displayAlarmSet(alarmType);

  Serial.print("********** ALARM: ");

  if (alarmType == ALARM_FAN_FAIL) {
    Serial.print("Fan Failure");
  } else if (alarmType == ALARM_OVER_TEMP) {
    Serial.print("Over Temperature");
  } else if (alarmType == ALARM_OVER_CURRENT) {
    Serial.print("Over Current");
  }
  Serial.println(" **********");
}

void clearAlarm() {
  alarmTriggeredFlag = false;
  displayAlarmClear();
  digitalWrite(pinDebugLED, LOW);
  Serial.println("********** Alarm(s) Cleared **********");
}

bool getAlarmFlag() { return alarmTriggeredFlag; }

void setCurrent(uint16_t current) {
  /*
    if (current >= MAX_CURRENT) {
      outputCurrent = MAX_CURRENT;
      Serial.println("MAX output reached.");
      return;
    } else {
      outputCurrent = current;
    }
  */
  outputCurrent = current;
  // I could make that calculation in one line, but it'll take me
  // ages to figure it out if I ever have to look at this again.
  float value = (float(current) + 0.5) / 1000.0;
  uint16_t outputCode = uint16_t((value / VREF) * 4095.0);
  setDAC(outputCode);

  Serial.print("Output current: ");
  Serial.print(value, 3);
  Serial.print(" A");
  Serial.println();
}

void setDAC(uint16_t output_value) {

  // outputValueDAC = output_value;

  delay(50); // REQUIRED !!!!

  Wire.begin();
  Wire.beginTransmission(MCP4725_ADDR);
  Wire.write(64);                       // cmd to update the DAC
  Wire.write(output_value >> 4);        // the 8 most significant bits...
  Wire.write((output_value & 15) << 4); // the 4 least significant bits...
  Wire.endTransmission();
  Wire.end();

  Serial.print("DAC output: ");
  Serial.print(output_value);
  Serial.println();
}

uint16_t getCurrent() { return outputCurrent; }

/*
void setDAC(uint16_t value) {

  if (value >= maxDAC) {
    value = maxDAC;
  }
  outputValueDAC = value;
  analogWrite(pinDacOutput, value);
  Serial.print("DAC output code: ");
  Serial.println(outputValueDAC);
}
*/

// uint16_t getValueDAC() { return outputValueDAC; }

/*
void setOutput(uint16_t desiredOutput) {
  float output = desiredOutput;
  output = desiredOutput * (3.3 / 4095.0) * 1000.0;
  Serial.print("Desired output: ");
  Serial.println(desiredOutput);

  Serial.print("Output: ");
  Serial.println(output);
  setDAC(uint16_t(desiredOutput));
}
*/