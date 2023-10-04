#ifndef _PINS_H_
#define _PINS_H_

/*
    Currently Free:
        12 (MISO, not used for display)
        A2
        A3
        A6 Analog only (fan tach charge cap something?)
*/

// TX   0
// RX   1
#define pinFanTach 2
#define pinFanPWM 3
#define pinFanEnable 4

#define pinEnableRelay A0
#define pinRangeRelay A1

#define pinRangeSwitch 5
#define pinOutputEnableSwitch 6
#define pinEncoderSwitch 7
#define pinEncoderA 8
#define pinEncoderB 9

#define DISPLAY_SS 10
#define DISPLAY_MOSI 11
//#define PIN_MISO     12      (not used for display)
#define DISPLAY_SCK 13



// i²c SCK      A4
// i²c SCL      A5

#define pinNTC A7

//  Signal     Pin    Display wire
// SPI SS       10      (Yellow)
// SPI MOSI     11      (Purple)
// SPI MISO     12      Not used
// SPI SCK      13      (Blue)

#endif
