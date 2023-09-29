#ifndef _PINS_H_
#define _PINS_H_

// Keep the noisy pins on the opposite side
// from the DAC and ADC as much as possible

/*
    Currently Free:
        D2
        A6/A7 Analog only (fan tach charge cap something?)
        D8
        *SPI 10/11/13 -- MISO (12) Not used for display
*/

#define pinOutputEnableSwitch A0

#define pinEncoderSwitch A1
#define pinEncoderA A2
#define pinEncoderB A3

// i²c SCK      A4
// i²c SCL      A5

// TX   0
// RX   1
#define pinRangeSwitch 4
#define pinFanEnable 5
#define pinAlarmLED 6 // Do we need this with the display anyway?
#define pinOutputEnableRelay 7

#define pinFanTach 3
#define pinFanPWM 9 // Timer 1

// SPI SS       10      (Yellow)
// SPI MOSI     11      (Purple)
// SPI MISO     12      Not used
// SPI SCK      13      (Blue)
#endif
