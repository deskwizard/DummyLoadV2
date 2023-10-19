#include "MCP4922.h"
#include "Arduino.h"

MCP4922::MCP4922(){};

void MCP4922::begin() {
  pinMode(CS_Pin, OUTPUT);
  digitalWrite(CS_Pin, HIGH);
  SPI.begin();
}

void MCP4922::begin(uint8_t cs_pin) {
  CS_Pin = cs_pin;
  begin();
}

void MCP4922::shutdown(bool state) {
  shutdown(CHAN_A, state);
  shutdown(CHAN_B, state);
}

void MCP4922::shutdown(bool channel, bool state) {
  shutdown_state[channel] = !state;
  update_DAC(channel);
}

void MCP4922::set_gain(bool gain_val) {
  set_gain(CHAN_A, gain_val);
  set_gain(CHAN_B, gain_val);
}

void MCP4922::set_gain(bool channel, bool gain_val) {
  gain_setting[channel] = gain_val;
  update_DAC(channel);
}

void MCP4922::set_buffered(bool state) {
  set_buffered(CHAN_A, state);
  set_buffered(CHAN_B, state);
}

void MCP4922::set_buffered(bool channel, bool state) {
  buffer_setting[channel] = state;
  update_DAC(channel);
}

void MCP4922::setDAC(uint16_t desired_value) {

  data_word[CHAN_A] = desired_value;
  data_word[CHAN_A] = desired_value;
  update_DAC(CHAN_A);
  update_DAC(CHAN_B);
}

void MCP4922::setDAC(bool channel, uint16_t desired_value) {

  data_word[channel] = desired_value;

  if (channel) {
    bitSet(data_word[channel], CHAN_BIT);
  } else {
    bitClear(data_word[channel], CHAN_BIT);
  }

  update_DAC(channel);
}

void MCP4922::update_DAC(bool channel) {

  digitalWrite(CS_Pin, LOW); // take the CS pin low to select the chip:
  delay(10);

  bitClear(data_word[channel], SHDN_BIT);
  bitClear(data_word[channel], GAIN_BIT);
  data_word[channel] =
      data_word[channel] | (shutdown_state[channel] << SHDN_BIT);
  data_word[channel] = data_word[channel] | (gain_setting[channel] << GAIN_BIT);

  SPI.transfer(data_word[channel] >> 8);
  SPI.transfer(data_word[channel] & 0xFF);

  delay(10);
  digitalWrite(CS_Pin, HIGH); // take the CS pin high to de-select the chip:
}
