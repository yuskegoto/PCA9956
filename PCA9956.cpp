#include "PCA9956.h"
// #define SERIAL_DEBUG

// Init PCA9956 library with the pointer of TwoWire instance Wire or Wire1
PCA9956::PCA9956(TwoWire *w) : wire(w)
{};

void PCA9956::init(uint8_t devAddress, uint8_t ledBrightness, bool enablePWM)
{
    _deviceAddress = devAddress;
    #ifdef SERIAL_DEBUG
    Serial.println();
    Serial.println(devAddress);
    #endif

    if (!enablePWM) ledMode1Setting(MODE1_SETTING_NO_INCREMENT); //disables all options...
    setLEDOutMode_all(LEDMODE_FULLOFF); // set leds to full off

    setLEDCurrent_all(ledBrightness);
    isPWM = false;
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

void PCA9956::i2cWrite(uint8_t slave_address, uint8_t *data, uint8_t num)
{
    wire->beginTransmission(slave_address);
    for (int i = 0; i < num; i++)
    {
        wire->write(*(data + i));
    }
    wire->endTransmission();
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
        uint8_t regAdr = uint8_t(LEDNo / 4) + LEDOUT0;
        uint8_t regVal = 1 << ((LEDNo % 4) * 2);

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

        uint8_t regAdr = uint8_t(LEDNo / 4) + LEDOUT0;
        
        // check other leds' status
        uint8_t registerValue = 0;

        uint8_t ledGroupe = (LEDNo / 4) * 4;
        for (uint8_t i; i < 4; i++)
        {
            if (ledGroupe + i != LEDNo && ledStatus[ledGroupe + i] == 0xFF)
            {
                registerValue |= 1 << (((ledGroupe + i) % 4) * 2);
            }
        }
        setLEDOutMode(regAdr, 0);        // TODO: must be implemented to turn off individually
        ledStatus[LEDNo] = 0;
    }
}

void PCA9956::setPWMMode_all()
{
    // set pwm auto INCREMENT
    ledMode1Setting(MODE1_SETTING_AUTO_INCREMENT_BRIGHTNESS);
    setLEDOutMode_all(LEDMODE_PWM);
    isPWM = true;

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
    if(LEDNo < PCA9965_NUM_LEDS)
    {
        if (!isPWM)
        {
            setPWMMode_all();
        }
        uint8_t cmd[2];
        cmd[0] = PWM0 + LEDNo;
        cmd[1] = PWMPower;

        i2cWrite(_deviceAddress, cmd, 2);
    }
}

// bool PCA9956::checkTempWarning()
// {

// }

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
// void PCA9956_Manager::setSectorAndLEDNo(uint8_t driverNo, uint8_t *sectorNos, uint8_t *sectorLEDNos)
void PCA9956_Manager::setSectorAndLEDNo(uint8_t driverNo, const uint8_t *sectorNos, const uint8_t *sectorLEDNos)
{
    for(uint8_t i = 0; i < PCA9965_NUM_LEDS; i ++)
    {
        deviceMap.device[driverNo].led[i].sectorNo = sectorNos[i];
        deviceMap.device[driverNo].led[i].ledNo = sectorLEDNos[i];
    }
    updateSector2DevNoMap();
}

// This fuction assumes that numbers of driver < number of sectors, so when you specify sector No, driver No can be determined.
uint8_t PCA9956_Manager::getDeviceNo(uint8_t sectorNo)
{
    // for (uint8_t devNo = 0; devNo < numDevices; devNo++)
    // {
    //     if (deviceMap.device[devNo].led[0].sectorNo <= sectorNo &&
    //         sectorNo <= deviceMap.device[devNo].led[PCA9965_NUM_LEDS - 1].sectorNo)
    //     {
    //         // for (uint8_t j = 0; j < PCA9965_NUM_LEDS; j++)
    //         // {
    //         //     Serial.print(deviceMap.driver[devNo].led[j].sectorNo);
    //         //     Serial.print(" ");
    //         // }
    //         // Serial.println("");
    //         return devNo;
    //     }
    // }

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
