#ifndef _PINS_H_
#define _PINS_H_

/*
    Currently Free:
        12 (MISO, not used for display)
        A3
        A4
        A5
        A7 Analog only
*/

// TX   0
// RX   1
#define pinFanTach 2
#define pinFanPWM 3
#define pinFanEnable 4

#define pinEnableRelay A1
#define pinRangeRelay A2

#define pinRangeSwitch 5
#define pinOutputEnableSwitch 6
#define pinEncoderSwitch 7
#define pinEncoderA 8
#define pinEncoderB 9


#define DISPLAY_CS 10
#define PIN_MOSI 11
#define PIN_MISO 12
#define PIN_SCK 13
#define DAC_CS A0
#define ADC_CS A3


// i²c SCK      A4
// i²c SCL      A5

#define pinNTC A6

//  Signal     Pin    Display wire
// SPI SS       10      (Yellow)
// SPI MOSI     11      (Purple)
// SPI MISO     12      Not used
// SPI SCK      13      (Blue)

#endif
