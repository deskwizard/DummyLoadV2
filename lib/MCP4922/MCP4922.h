#ifndef MCP4922_H
#define MCP4922_H

#include <SPI.h>


#define CHAN_BIT 15
#define BUF_BIT 14
#define GAIN_BIT 13
#define SHDN_BIT 12

#define CHAN_A 0
#define CHAN_B 1

#define GAIN1X 1
#define GAIN2X 0

#define BUFFERED 1
#define UNBUFFERED 0


class MCP4922 {

  public:
    MCP4922();
    void begin();
    void begin(uint8_t cs_pin);

    void shutdown(bool state);
    void shutdown(bool channel, bool state);

    void set_gain(bool gain_val);
    void set_gain(bool channel, bool gain_val);

    void set_buffered(bool state);
    void set_buffered(bool channel, bool state);

    void setDAC(uint16_t desired_value);
    void setDAC(bool channel, uint16_t desired_value);
    


  private:
    void update_DAC(bool channel);

    uint8_t CS_Pin = 10;
    uint16_t data_word[2] = {0, 0};
    bool shutdown_state[2] = {0, 0};
    bool gain_setting[2] = {GAIN1X, GAIN1X};       // defaults to 1X
    bool buffer_setting[2] = {UNBUFFERED, UNBUFFERED};       // defaults to 1X
};



#endif
