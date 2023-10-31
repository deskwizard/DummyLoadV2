#include "control.h"
#include "MCP4922.h"
#include "SPI.h"
#include "cooling.h"
#include "display.h"

MCP4922 DAC;

uint16_t outputCurrent = 0;
bool outputEnabled = false;
bool outputRange = LOW;
bool alarmTriggeredFlag = false;

uint64_t voltageRunningTotal = 0;

void configureControls() {

  DAC.begin(DAC_CS); // Custom CS pin

  // Enable channel A, leave channel B in shutdown
  DAC.shutdown(CHAN_A, false);

  pinMode(ADC_CS, OUTPUT);
  digitalWrite(ADC_CS, HIGH);

  pinMode(pinEnableRelay, OUTPUT);
  pinMode(pinRangeRelay, OUTPUT);
  digitalWrite(pinEnableRelay, LOW);
  digitalWrite(pinRangeRelay, LOW);
}

void setOutputState(bool state) {

  digitalWrite(pinEnableRelay, state);

  outputEnabled = state;
  displayValue(DISPLAY_MAIN, getCurrent());

  Serial.print("Output state changed to: ");
  Serial.println(state);
}

bool getOutputState() { return outputEnabled; }

void setOutputRange(bool state) {

  outputRange = state;

  Serial.print("Output Range changed to: ");

  if (outputRange == RANGE_HIGH) {

    // Switching from low range to high range
    Serial.println("High");

  } else {
    // switching from high range to low range
    Serial.println("Low");

    if (outputCurrent > getMaxCurrent()) {
      outputCurrent = getMaxCurrent();
    }
  }

  setCurrent(outputCurrent);

  digitalWrite(pinRangeRelay, state);
}

bool getOutputRange() { return outputRange; }

void setAlarm(uint8_t alarmType) {
  alarmTriggeredFlag = true;

  // No matter what, turn output off, set fan to maximum speed
  setOutputState(OUTPUT_OFF);
  setFanPWM(255);

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
  setDisplayMode(DISPLAY_MODE_VALUE);
  Serial.println("********** Alarm(s) Cleared **********");
}

bool getAlarmFlag() { return alarmTriggeredFlag; }

void setCurrent(uint16_t current) {

  outputCurrent = current;

  // I could make that calculation in one line, but it'll take me
  // ages to figure it out if I ever have to look at this again.
  float value = (float(current) + 0.5) / 1000.0;
  uint16_t outputCode = uint16_t((value / VREF) * 4095.0);

  if (outputRange == RANGE_LOW) {
    outputCode = outputCode * 10;
  }

  setDAC(outputCode);

  displayValue(DISPLAY_MAIN, outputCurrent);

  Serial.print("Output current: ");
  Serial.print(value, 3);
  Serial.print(" A");
  Serial.println();
}

void setDAC(uint16_t output_value) {
  DAC.setDAC(CHAN_A, output_value);
  Serial.print("DAC output: ");
  Serial.print(output_value);
  Serial.println();
}

uint16_t getCurrent() { return outputCurrent; }

uint16_t getMaxCurrent() {
  if (outputRange == RANGE_LOW) {
    return MAX_CURRENT / 10;
  } else {
    return MAX_CURRENT;
  }
}

void readVoltage() {

  static uint8_t readingIndex = 0; // Index of the current reading

  static uint16_t readings[VOLT_READ_COUNT] = {0};

  uint16_t read = readChannelSE(ADC_VOLTAGE_CHANNEL);

  /*
    Serial.print(read);
    if (read >= 4095) {
      return;
    }
    //Serial.println();
  */

  // Subtract the last reading
  voltageRunningTotal = voltageRunningTotal - readings[readingIndex];

  readings[readingIndex] = read; // Read from the sensor

  // Add the reading to the running total
  voltageRunningTotal = voltageRunningTotal + readings[readingIndex];

  readingIndex++; // Advance to the next position in the array

  // If we're at the end of the array, wrap around to the beginning
  if (readingIndex >= VOLT_READ_COUNT) {
    readingIndex = 0;
  }
}

uint16_t getVoltage() { return voltageRunningTotal / VOLT_READ_COUNT; }

uint16_t readChannelSE(uint8_t channel) {

  delay(1); // Required

  /*
    Serial.print("Channel ");
    Serial.print(channel);
    Serial.print(": ");
  */

  // Start and SE bits always set (D2 is 'don't care' for MCP3302)
  uint8_t commandByte = 0x18;

  commandByte = commandByte + channel;

  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

  digitalWrite(ADC_CS, LOW);

  // Send command byte
  SPI.transfer(commandByte);

  // Receive high byte and shift to be the high 8 bits
  uint16_t reading = SPI.transfer(0x00) << 8;

  // Receive low byte into lower 8 bits
  reading |= SPI.transfer(0x00);

  digitalWrite(ADC_CS, HIGH);

  SPI.endTransaction();

  uint16_t calculated = uint16_t((float)reading * (VREF / 4.096) + 0.5);

  calculated = calculated >> 1;

  return calculated;
}

void testDAC() {

  static uint16_t out_a = 0;
  static uint16_t out_b = 0;

  unsigned long currentMillis = millis();
  static unsigned long previousMillis = 0;

  if ((unsigned long)(currentMillis - previousMillis) >= 250) {

    if (out_a < 4095) {
      out_a = out_a + 10;
    } else {
      out_a = 0;
    }

    DAC.setDAC(CHAN_A, out_a);

    Serial.print("A: ");
    Serial.print(out_a);
    Serial.print(" ");
    Serial.println(float(4096 / (VREF * 1000)) + 1 * out_a);

    if (out_b < 4095) {
      out_b = out_b + 25;
    } else {
      out_b = 0;
    }

    DAC.setDAC(CHAN_B, out_b);
    Serial.print("B: ");
    Serial.println(out_b);

    Serial.println();

    previousMillis = currentMillis;
  }
}
