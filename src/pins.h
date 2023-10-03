#ifndef _PINS_H_
#define _PINS_H_

// Keep the noisy pins on the opposite side
// from the DAC and ADC as much as possible

/*
    Currently Free:
        D2
        D6
        12 (MISO, not used for display)
        A6 Analog only (fan tach charge cap something?)
*/

// TX   0
// RX   1
#define pinFanTach 3
#define pinFanEnable 4
#define pinFanPWM 5 // Timer 0

#define pinEnableRelay 7
#define pinRangeSwitch 8

// SPI SS       10      (Yellow)
// SPI MOSI     11      (Purple)
// SPI MISO     12      Not used
// SPI SCK      13      (Blue)

#define DISPLAY_SS 10
#define DISPLAY_MOSI 11
// 12
#define DISPLAY_SCK 13
#define pinOutputEnableSwitch A0

#define pinEncoderSwitch A1
#define pinEncoderA A2
#define pinEncoderB A3

// i²c SCK      A4
// i²c SCL      A5

#define pinNTC A7

#endif
