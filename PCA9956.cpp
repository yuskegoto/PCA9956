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
 *
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Yuske Goto         2020/12/10        0.1             Added multiple driver and sector management class.
 * </pre>
 *
 */

#include "PCA9956.h"
// #define SERIAL_DEBUG

// Init PCA9956 library with the pointer of TwoWire instance Wire or Wire1
PCA9956::PCA9956(TwoWire *w) : wire(w)
{};

void PCA9956::init(uint8_t devAddress, uint8_t ledBrightness, bool enablePWM, bool resetStatus_all)
{
    _deviceAddress = devAddress;
    #ifdef SERIAL_DEBUG
    Serial.println();
    Serial.println(devAddress);
    #endif

    if (resetStatus_all)
    {
        resetAllDevices();
    }

    if (!enablePWM)
    {
        ledMode1Setting(MODE1_SETTING_NO_INCREMENT);
        setLEDOutMode_all(LEDMODE_FULLOFF); // set leds to full off

        isPWM = false;
    }
    else
    {
        setPWMMode_all(true);
    }

    setLEDCurrent_all(ledBrightness);
}

// send software reset to all devices
// Resetting the driver several times causes the chip to halt
void PCA9956::resetAllDevices()
{
    uint8_t data[] = {PCA9956_RESET_ALL};
    i2cWrite(PCA9956_I2C_GENERAL_CALL, data, 1);
    delay(1);
}

void PCA9956::i2cWrite(uint8_t slave_address, uint8_t *data, uint8_t dataLength)
{
    wire->beginTransmission(slave_address);
    for (int i = 0; i < dataLength; i++)
    {
        wire->write(*(data + i));
    }
    wire->endTransmission();
}

// i2c scanner taken from here: https://playground.arduino.cc/Main/I2cScanner
uint8_t PCA9956::i2cScan(uint8_t startAddress)
{
    uint8_t foundAddress = 0;
    for (uint8_t address = startAddress; address < 127; address++)
    {
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        wire->beginTransmission(address);
        uint8_t error = wire->endTransmission();

        if (error == 0)
        {
            Serial.print("\nI2C device found at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.println(address, HEX);
            foundAddress = address;
            break;
        }
        else if (error == 4)
        {
            Serial.print("Unknown error at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.println(address, HEX);
        }
    }
    return foundAddress;
}

void PCA9956::setPWMMode_all(bool setAllLEDsOff)
{
    isPWM = true;
    if(setAllLEDsOff)
    {
        uint8_t pattern[PCA9965_NUM_LEDS] = {0};
        setLEDPattern(pattern);
    }

    // set pwm auto INCREMENT
    ledMode1Setting(MODE1_SETTING_AUTO_INCREMENT_BRIGHTNESS);
    setLEDOutMode_all(LEDMODE_PWM);
}

// LED No: 0 - 23
void PCA9956::setLEDCurrent(uint8_t LEDNo, uint8_t irefFactor)
{
    if (LEDNo < PCA9965_NUM_LEDS)
    {
        uint8_t cmd[2];
        cmd[0] = LEDNo + IREF0;
        cmd[1] = irefFactor;
        i2cWrite(_deviceAddress, cmd, sizeof(cmd));
    }
}

void PCA9956::setLEDCurrent_all(uint8_t irefFactor)
{
    // set current auto INCREMENT
    ledMode1Setting(MODE1_SETTING_AUTO_INCREMENT_IREF);
    uint8_t cmd[25];
    cmd[0] = IREF0 | AUTO_INCREMENT_BIT;
    for(uint8_t i = 1; i < 25; i++)
        cmd[i] = irefFactor;

    i2cWrite(_deviceAddress, cmd, sizeof(cmd));

    // stop auto INCREMENT
    ledMode1Setting(MODE1_SETTING_NO_INCREMENT);
}

//MODE1 reg setting
void PCA9956::ledMode1Setting(uint8_t regsetting)
{
    uint8_t cmd[2];
    cmd[0] = MODE1;
    cmd[1] = regsetting;

    i2cWrite(_deviceAddress, cmd, 2);
}

/***************************************************************************
 * @brief to global blink to work you need to set
 * PWM0 - 23 (0Ah - 21h) to 0xFF or any value above 0 to see the result
 * MODE2 01h register DMBLINK bit (bit 5) to 1
 * LEDOUT1 to LEDOUT5 (02h - 07h) to 0b11111111 = LDRx => 11
 * GRPFREQ (09h) for group blink frequency setting. From 0x00 for 15.26Hz to 0xFF for 16.8S
 * GRPPWM (08h) for Group duty cycle setting.
 * @param freq: GRPFREQ setting
 * @param duty: GRPPWM setting, default is 0x80 = 50%
 ***************************************************************************/
void PCA9956::setBlinking(uint8_t freq, uint8_t duty)
{
    // for (uint8_t i = 0; i <= PCA9965_NUM_LEDS; i++)
    // {
    //     pwmLED(i, 0xFF);
    // }
    setSingleRegister(PWMALL, 0xFF);
    // ledMode1Setting(MODE1_SETTING_AUTO_INCREMENT_BRIGHTNESS);
    // uint8_t pattern[PCA9965_NUM_LEDS] = {0xFF};
    // setLEDPattern(pattern);

    isPWM = false;
    // ledMode2Setting(MODE2_DMBLINK_BLINKING);
    setSingleRegister(MODE2, MODE2_DMBLINK_BLINKING);

    // ledMode1Setting(MODE1_SETTING_NO_INCREMENT);
    setLEDOutMode_all(LEDMODE_GROUP_DIMMING);
    setSingleRegister(GRPFREQ, freq); // Group dimming frequency setting
    setSingleRegister(GRPPWM, duty); // Group dimming duty setting
}

void PCA9956::setSingleRegister(uint8_t registerAddress, uint8_t command)
{
    uint8_t cmd[2];
    cmd[0] = registerAddress;
    cmd[1] = command;

    i2cWrite(_deviceAddress, cmd, 2);
}

void PCA9956::setLEDOutMode_all(uint8_t mode)
{
    uint8_t cmd[2];
    cmd[1] = mode;

    for(uint8_t i = 0; i < 6; i ++)
    {
        cmd[0] = LEDOUT0 + i;
        i2cWrite(_deviceAddress, cmd, sizeof(cmd));
    }
}

void PCA9956::setLEDOutMode(uint8_t regAdr, uint8_t mode)
{

    uint8_t cmd[2];
    cmd[0] = regAdr;
    cmd[1] = mode;

    i2cWrite(_deviceAddress, cmd, sizeof(cmd));

#ifdef SERIAL_DEBUG
    Serial.print(_deviceAddress, HEX);
    Serial.print(" ");
    Serial.print(regAdr, HEX);
    Serial.print(" ");
    Serial.print(mode, BIN);
    Serial.println("");
#endif
}

void PCA9956::onLED(uint8_t LEDNo)
{
    if (LEDNo < PCA9965_NUM_LEDS)
    {
        if (isPWM)
            setLEDOutMode_all(LEDMODE_FULLOFF);
        isPWM = false;

        #ifdef SERIAL_DEBUG
        Serial.print("on  ");
        #endif
        uint8_t LEDgroup = uint8_t(LEDNo / 4);
        uint8_t regAdr = LEDgroup + LEDOUT0;
        uint8_t regVal = 1 << ((LEDNo % 4) * 2);

        for (uint8_t i = 0; i < 4; i++)
        {
            uint8_t led = (LEDgroup * 4) + i;
            if (led != LEDNo)
            {
                if(ledStatus[led] == 0xFF)
                {
                    regVal |= 1 << (i * 2);
                }
            }
        }

        setLEDOutMode(regAdr, regVal);
        ledStatus[LEDNo] = 0xFF;
    }
}

// LED No. 0 -23
void PCA9956::offLED(uint8_t LEDNo)
{
    if (LEDNo < PCA9965_NUM_LEDS)
    {
        if (isPWM)
            setLEDOutMode_all(LEDMODE_FULLOFF);
        isPWM = false;

        #ifdef SERIAL_DEBUG
        Serial.print("off ");
        #endif

        uint8_t LEDGroup = (LEDNo / 4);
        uint8_t regAdr = LEDGroup + LEDOUT0;
        
        // check other leds' status
        uint8_t registerValue = 0;

        for (uint8_t i = 0; i < 4; i++)
        {
            uint8_t led = (LEDGroup * 4) + i;
            if (led != LEDNo && ledStatus[led] == 0xFF)
            {
                registerValue |= 1 << (i * 2);
            }
        }
        // Serial.println(registerValue, BIN);
        setLEDOutMode(regAdr, registerValue);
        ledStatus[LEDNo] = 0;
    }
}

// Sets pattern with uint8_t array [0-255, 0-255, ...x24]
// the length of the pattern array must be 24 otherwise it wil crash!
void PCA9956::setLEDPattern(uint8_t *pattern)
{
    if(!isPWM) setPWMMode_all();

    #ifdef SERIAL_DEBUG
    Serial.print("pwm0: ");
    Serial.println(pattern[0]);
    #endif


    uint8_t cmd[25];
    cmd[0] = PWM0 | AUTO_INCREMENT_BIT;
    for (uint8_t i = 1; i < 25; i++)
    {
        cmd[i] = pattern[i - 1];
    }

    i2cWrite(_deviceAddress, cmd, sizeof(cmd));

    // stores led status
    for (uint8_t i = 0; i < PCA9965_NUM_LEDS; i++) ledStatus[i] = pattern[i];
}

// LED No: 0 - 23
void PCA9956::pwmLED(uint8_t LEDNo, uint8_t PWMPower)
{
    if (!isPWM)
    {
        setPWMMode_all();
    }

    if(LEDNo < PCA9965_NUM_LEDS)
    {
        uint8_t cmd[2];
        cmd[0] = PWM0 + LEDNo;
        cmd[1] = PWMPower;

        i2cWrite(_deviceAddress, cmd, 2);
    }
}

// read out one byte from regester
uint8_t PCA9956::readRegisterStatus(uint8_t regAddress)
{
    wire->beginTransmission(_deviceAddress);
    wire->write(regAddress);
    wire->endTransmission();

    Wire.requestFrom(_deviceAddress, 1);
    while (Wire.available() < 1)
        delay(1);
    uint8_t status = Wire.read();

    return status;
}

void PCA9956::clearMode2Error()
{
    uint8_t cmd[2];
    cmd[0] = MODE2;
    cmd[1] = MODE2_CLEARERROR;

    i2cWrite(_deviceAddress, cmd, 2);
}

// LEDGroup : 0 - 5 whereas 0: LED0 - LED3
// Retruns 0 if no error was detected
// See PCA9956 P.25 for detailed status
uint8_t PCA9956::getLEDErrorStatus(uint8_t LEDGroup)
{
    uint8_t status = readRegisterStatus(MODE2);
    // Serial.println(status, BIN);
    if (status & MODE2_LED_ERROR)
    {
        status = readRegisterStatus(ERROR_LED0_3 + LEDGroup);
        // Serial.println(status, BIN);
        return status;
    }
    return 0;
}

bool PCA9956::checkTempWarning()
{
    // Serial.println("mode2 status: ");
    clearMode2Error();
    uint8_t status = readRegisterStatus(MODE2);
    // Serial.println(status, BIN);
    if(status & MODE2_OVERTEMPERATURE)
    {
        // Serial.println("over temp!");
        return true;
    }
    // Serial.println();

    return false;
}

/****************************** Manager Functions ***********************************/
PCA9956_Manager::PCA9956_Manager(uint8_t num_sectors, uint8_t num_devices)
{
    numSectors = num_sectors;
    numDevices = num_devices;
    // sector2AddressMap = (uint8_t*)malloc(sizeof(uint8_t)* num_sectors);
    sector2DevNoMap = new uint8_t[num_sectors];
}

// destructor
PCA9956_Manager::~PCA9956_Manager()
{
    free(sector2DevNoMap);
}


void PCA9956_Manager::setAddress(uint8_t driverNo, uint8_t chipAddress)
{
    deviceMap.device[driverNo].address = chipAddress;
}

// Defines all 24 sectorNo and LEDNo on one chip of driver at once, therefore they must be uint8_t array with 24 elements: sectorNo[24], ledNo_Arm[24]
void PCA9956_Manager::setSectorAndLEDNo(uint8_t driverNo, const uint8_t *sectorNos, const uint8_t *sectorLEDNos)
{
    for(uint8_t i = 0; i < PCA9965_NUM_LEDS; i ++)
    {
        deviceMap.device[driverNo].led[i].sectorNo = sectorNos[i];
        deviceMap.device[driverNo].led[i].ledNo = sectorLEDNos[i];
    }
    updateSector2DevNoMap();
}

uint8_t PCA9956_Manager::getDeviceNo(uint8_t sectorNo)
{
    return sector2DevNoMap[sectorNo];
}

// Function to retrieve led No. of the chip from sector No and sector LED No.
uint8_t PCA9956_Manager::getLEDNo(uint8_t sectorNo, uint8_t sectorLEDNo)
{
    uint8_t devNo = getDeviceNo(sectorNo);
    for (uint8_t ledNo = 0; ledNo < PCA9965_NUM_LEDS; ledNo++)
    {
        // Serial.print(" ");
        // Serial.print(deviceMap.device[devNo].led[ledNo].sectorNo);
        // Serial.print(":");
        // Serial.print(deviceMap.device[devNo].led[ledNo].ledNo);
        // Serial.print(":");
        if (deviceMap.device[devNo].led[ledNo].ledNo == sectorLEDNo &&
            deviceMap.device[devNo].led[ledNo].sectorNo == sectorNo)
            // Serial.println("");
            return ledNo;
    }
    // Serial.println("");
    return 0xFF;
}

uint8_t PCA9956_Manager::getDeviceAddress(uint8_t deviceNo)
{
    return deviceMap.device[deviceNo].address;
}

uint8_t PCA9956_Manager::getDeviceAddressFromSectorNo(uint8_t sectorNo)
{
    uint8_t devNo = sector2DevNoMap[sectorNo];
    return getDeviceAddress(devNo);
}

void PCA9956_Manager::updateSector2DevNoMap()
{
    for(uint8_t devNo = 0; devNo < numDevices; devNo ++)
    {
        for (uint8_t ledNo = 0; ledNo < PCA9965_NUM_LEDS; ledNo ++)
        {
            uint8_t sectorNo = deviceMap.device[devNo].led[ledNo].sectorNo;
            sector2DevNoMap[sectorNo] = devNo;
        }
    }
}
