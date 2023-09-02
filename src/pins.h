#ifndef _PINS_H_
#define _PINS_H_

// Lets try to keep the noisy pins on the opposite side
// from the DAC and ADC

/*
    Currently Free:
        D10
        A1
        A4
        A5
        A6
        A7
*/


#define pinEncoderA D0
#define pinEncoderB D1
#define pinEncoderSwitch D2

#define pinRangeSwitch D3

#define displaySerialRX D4
#define displaySerialTX D5

#define pinFanEnable D6
#define pinFanPWM D7
#define pinFanTach D8

#define pinEnableSwitch D9

#define pinDebugLED D11

#define debugSerialRX D12
#define debugSerialTX D13

#define pinNTC A0
#define pinOutputEnable A2
#define pinDacOutput A3

#endif
