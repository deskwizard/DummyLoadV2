#ifndef _PINS_H_
#define _PINS_H_

// Lets try to keep the noisy pins on the opposite side
// from the DAC and ADC

/*
    Currently Free:
        D7
        D10
        A0 - A6

*/

#define pinEncoderSwitch A4
#define pinEncoderA A5
#define pinEncoderB A6

#define pinRangeSwitch D3

#define displaySerialRX D0
#define displaySerialTX D1

#define pinFanEnable D6
#define pinFanTach D8

#define pinEnableSwitch D9

#define pinDebugLED D11

#define debugSerialRX D12
#define debugSerialTX D13

#define pinOutputEnable A7

#endif
