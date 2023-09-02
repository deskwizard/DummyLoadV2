#ifndef _PINS_H_
#define _PINS_H_

// Lets try to keep the noisy pins on the opposite side
// from the DAC and ADC

/*
    Currently Free:
    D2
    D4
    D5
    A5
    A6
    A7

    Needed:
        - Range switch input ?
*/

#define serialRX D0
#define serialTX D1

#define pinRangeSwitch D3

#define pinNTC A0

#define pinOutputStateLED A1
#define pinOutputEnable A2
#define pinDacOutput A3

#define pinFanEnable D6
#define pinFanPWM D7
#define pinFanTach D8

#define pinEnableSwitch D9

#define pinEncoderSwitch D10
#define pinEncoderA D11
#define pinEncoderB D12

#define pinDebugLED D13

#endif
