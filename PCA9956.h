#ifndef _PCA9956_H_
#define _PCA9956_H_

#include <Arduino.h>
#include <Wire.h>

//PCA9956 registor addresses
#define MODE1 0x00
#define MODE1_SETTING_NO_INCREMENT 0x00
#define AUTO_INCREMENT_BIT 0b10000000
#define MODE1_SETTING_AUTO_INCREMENT_BRIGHTNESS 0b10100000
#define MODE1_SETTING_AUTO_INCREMENT_IREF 0b11000000
#define MODE2 0x01
#define PWM0 0x0A
#define PWM8 0x12
#define PWM23 0x21

#define GRPPWM 0x0A
#define GRPFREQ 0x0B
#define LEDOUT0 0x02
#define IREF0 0x22

#define LEDMODE_FULLOFF 0x00      //full off
#define LEDMODE_FULLON 0b01010101 //full on
#define LEDMODE_PWM 0b10101010    //control over pwm
#define LEDMODE_PWM_GROUPE_DIMMING 0b11111111    //pwm + groupe dimming

#define PCA9965_NUM_LEDS 24 // Fixed value

class PCA9956{
    public:
        PCA9956(TwoWire*);  //Initializer
        void init(uint8_t devAddress, uint8_t ledBrightness, bool enablePWM = false);
        uint8_t i2cScan(uint8_t startAddress = 1);
        // Turns on individual LED
        void onLED(uint8_t LEDNo);
        // Turns off individual LED
        void offLED(uint8_t LEDNo);
        // individually controls led with pwm
        void pwmLED(uint8_t LEDNo, uint8_t PWMPower);
        // Controls all 24 leds at once with pattern uint8_t[0-255, 0-255....]
        void setLEDPattern(uint8_t *LEDPattern);
        // Sets individual current
        void setLEDCurrent(uint8_t ledNo, uint8_t iref);
        void setPWMMode_all();
        void setLEDOutMode_all(uint8_t mode);

        bool isPWM;
        uint8_t ledStatus[PCA9965_NUM_LEDS];
        uint8_t _deviceAddress = 0x3F;       // when all i2c address config pins are nc/high-impedance state, the device address becomes 0x3F

    private:
        void ledMode1Setting(uint8_t regsetting);
        void setLEDCurrent_all(uint8_t iref);
        void setLEDOutMode(uint8_t registorAddress, uint8_t mode);
        void i2cWrite(uint8_t slave_address, uint8_t *data, uint8_t num);
        TwoWire *wire;
};

#endif