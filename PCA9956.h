// Licensed under the MIT license. See LICENSE file in the project root for full license information.
/**
 * \par Copyright (C), 2020, Yuske Goto
 * \class PCA9956
 * \brief   Driver for PCA9956
 * @file    PCA9956.h
 * @author  Yuske Goto
 * @version V0.1
 * @date    2020/12/10
 * @brief   Header for PCA9956.cpp module
 *
 * \par Description
 * This file is a driver for PCA9956 LED driver module.
 * See detail for the spec-sheet below
 * https://www.nxp.com/docs/en/data-sheet/PCA9956B.pdf *
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Yuske Goto         2020/12/10        0.1             Added multiple driver and sector management class.
 * </pre>
 *
 */

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
#define MODE2_OVERTEMPERATURE 0b10000000
#define MODE2_LED_ERROR 0b1000000
#define MODE2_CLEARERROR 0b10000
#define MODE2_DMBLINK_BLINKING 0b100000
#define ERROR_LED0_3 0x41
#define ERROR_LED4_7 0x42
#define ERROR_LED8_11 0x43
#define ERROR_LED12_15 0x44
#define ERROR_LED16_19 0x45
#define ERROR_LED20_23 0x46
#define ERROR_OPEN_CIRCUIT 0b10101010
#define ERROR_SHORT_CIRCUIT 0b01010101
#define PCA9956_I2C_GENERAL_CALL 0x0
#define PCA9956_RESET_ALL 0x6

#define PWM0 0x0A
#define PWMALL 0x3F

#define GRPPWM 0x08
#define GRPFREQ 0x09
#define LEDOUT0 0x02
#define IREF0 0x22

#define LEDMODE_FULLOFF 0x00      //full off
#define LEDMODE_FULLON 0b01010101 //full on
#define LEDMODE_PWM 0b10101010    //control over pwm
#define LEDMODE_GROUP_DIMMING 0b11111111 // pwm + group dimming / blinking

#define PCA9965_NUM_LEDS 24 // Fixed value

class PCA9956{
    public:
        PCA9956(TwoWire*);  //Initializer

        // Resetting the driver several times causes the chips to halt
        void init(uint8_t devAddress, uint8_t ledBrightness, bool enablePWM = false, bool resetStatus_all = false);
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
        void setLEDCurrent(uint8_t ledNo, uint8_t irefFactor);
        void setPWMMode_all(bool setAllLEDsOff = false);
        void setLEDOutMode_all(uint8_t mode);
        bool checkTempWarning();
        uint8_t getLEDErrorStatus(uint8_t LEDGroup = ERROR_LED0_3);
        // send software reset to all devices
        // Resetting the driver several times causes the chips to halt
        void resetAllDevices();
        void setBlinking(uint8_t freq, uint8_t duty = 0x80);
        //void setGroupFrequency(uint8_t freq);

        bool isPWM;
        uint8_t ledStatus[PCA9965_NUM_LEDS];
        uint8_t _deviceAddress = 0x3F;       // when all i2c address config pins are nc/high-impedance state, the device address becomes 0x3F

    private:
        void ledMode1Setting(uint8_t regsetting);
        void setLEDCurrent_all(uint8_t iref);
        void setLEDOutMode(uint8_t registorAddress, uint8_t mode);
        void i2cWrite(uint8_t slave_address, uint8_t *data, uint8_t dataLength);
        uint8_t readRegisterStatus(uint8_t regAddress);
        void clearMode2Error();

        void setSingleRegister(uint8_t registerAddress, uint8_t command);

        TwoWire *wire;
};

#define NUM_PCA9956s 10

struct PCA9956_LED{
    uint8_t sectorNo;
    uint8_t ledNo;
};

struct PCA9956_Chip
{
    struct PCA9956_LED led[PCA9965_NUM_LEDS];
    uint8_t address;
};

struct PCA9956_Group
{
    struct PCA9956_Chip device[NUM_PCA9956s];
};

class PCA9956_Manager{
    public:
        PCA9956_Manager(uint8_t num_sectors, uint8_t num_devices = NUM_PCA9956s);
        ~PCA9956_Manager(); // destructor
        void setAddress(uint8_t driverNo, uint8_t chipAddress);
        // void setSectorAndLEDNo(uint8_t driverNo, uint8_t *sectorNos, uint8_t *sectorLEDNos); // Defines all 24 sectorNo and LEDNo on one chip of driver at once, therefore they must be uint8_t array with 24 elements: sectorNo[24], ledNo_Sector[24]
        void setSectorAndLEDNo(uint8_t driverNo, const uint8_t *sectorNos, const uint8_t *sectorLEDNos); // Defines all 24 sectorNo and LEDNo on one chip of driver at once, therefore they must be uint8_t array with 24 elements: sectorNo[24], ledNo_Sector[24]
                                                                                                         // sector No and led No on the sector must be defined for each of led channels on the chip
        uint8_t getDeviceNo(uint8_t sectorNo);
        uint8_t getLEDNo(uint8_t sectorNo, uint8_t sectorLEDNo); // return: led No. on the chip, not the actual No. on the arm
        uint8_t getDeviceAddress(uint8_t deviceNo);
        uint8_t getDeviceAddressFromSectorNo(uint8_t sectorNo);

        uint8_t numDevices = NUM_PCA9956s;
        uint8_t numSectors = NUM_PCA9956s;

    private:
        void updateSector2DevNoMap();
        PCA9956_Group deviceMap;
        uint8_t* sector2DevNoMap;
};


#endif