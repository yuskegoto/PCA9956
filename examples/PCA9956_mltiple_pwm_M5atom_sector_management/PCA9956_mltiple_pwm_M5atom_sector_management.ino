// Not so friendly data sheet: https://www.nxp.com/docs/en/data-sheet/PCA9956B.pdf
#include <Arduino.h>
#include <Wire.h>

#include "PCA9956.h"
#include "Setting.h"

#define M5ATOM

#ifdef M5ATOM
#include "M5Atom.h"
#endif

PCA9956 ledDrivers[2]{{&Wire}, {&Wire}}; // if you have multiple drivers, init this way...

#ifdef M5ATOM
bool blink = false;
#endif

#define NUM_SECTORS 4 //13
#define NUM_DEVICES 2
PCA9956_Manager pcaManager(NUM_SECTORS, NUM_DEVICES);

void setup()
{
    pcaManager.setAddress(0, DEV_ADDRESS1);
    pcaManager.setAddress(1, DEV_ADDRESS2);
    // pcaManager.setAddress(2, DEV_ADDRESS3);
    // pcaManager.setAddress(3, DEV_ADDRESS4);
    // pcaManager.setAddress(4, DEV_ADDRESS5);
    // pcaManager.setAddress(5, DEV_ADDRESS6);
    // pcaManager.setAddress(6, DEV_ADDRESS7);
    // pcaManager.setAddress(7, DEV_ADDRESS8);
    // pcaManager.setAddress(8, DEV_ADDRESS9);
    // pcaManager.setAddress(9, DEV_ADDRESS10);

    pcaManager.setSectorAndLEDNo(0, DEV1_Groupe, DEV1_ledNo);
    pcaManager.setSectorAndLEDNo(1, DEV2_Groupe, DEV2_ledNo);
    // pcaManager.setSectorAndLEDNo(2, DEV3_Groupe, DEV3_ledNo);
    // pcaManager.setSectorAndLEDNo(3, DEV4_Groupe, DEV3_ledNo);
    // pcaManager.setSectorAndLEDNo(4, DEV5_Groupe, DEV3_ledNo);
    // pcaManager.setSectorAndLEDNo(5, DEV6_Groupe, DEV3_ledNo);
    // pcaManager.setSectorAndLEDNo(6, DEV7_Groupe, DEV3_ledNo);
    // pcaManager.setSectorAndLEDNo(7, DEV8_Groupe, DEV3_ledNo);
    // pcaManager.setSectorAndLEDNo(8, DEV9_Groupe, DEV3_ledNo);
    // pcaManager.setSectorAndLEDNo(9, DEV10_Groupe, DEV3_ledNo);

#ifdef M5ATOM
    M5.begin(true, false, true); // seems like m5 inits wire, which is not comvenient so disables it first
    delay(50);
    M5.dis.drawpix(0, 0x000000);
#else
    Serial.begin(9600);
#endif

    Wire.begin(21, 22); // Wire needs to be initialized separately...

    ledDrivers[0].init(DEV_ADDRESS1, LED_BRIGHTNESS);
    ledDrivers[1].init(DEV_ADDRESS2, LED_BRIGHTNESS);

    // Serial.println("");
    // for (uint8_t i = 0; i < pcaManager.numDevices; i++)
    // {
    //     Serial.print(pcaManager.getDeviceAddress(i), HEX);
    //     Serial.print(" ");
    // }
    // Serial.println("");

    // Serial.println("get driver id");
    // Serial.println(pcaManager.getDeviceNo(0));
    // Serial.println(pcaManager.getDeviceNo(1));
    // Serial.println(pcaManager.getDeviceNo(2));
    // Serial.println(pcaManager.getDeviceNo(3));

    // for (uint8_t i = 0; i < NUM_SECTORS; i++)
    // {
    //     Serial.print(pcaManager.getDeviceAddressFromSectorNo(i), HEX);
    //     Serial.print(" ");
    // }
    // Serial.println("");

    // Serial.println("printing led No of the sector0");
    // for (uint8_t i = 0; i < 12; i++)
    // {
    //     Serial.print(i);
    //     Serial.print(":");
    //     Serial.print(pcaManager.getLEDNo(0, i));
    //     Serial.print(" ");
    // }
    // Serial.println("");

    // Serial.println("printing led No of the sector2");
    // for (uint8_t i = 0; i < PCA9965_NUM_LEDS; i++)
    // {

    //     // Serial.print(" ");
    //     Serial.print(i);
    //     Serial.print(":");
    //     Serial.print(pcaManager.getLEDNo(2, i));
    //     Serial.print(" ");
    //     // Serial.println("");
    // }
    // Serial.println("");

    // Serial.println("printing led No of the sector3");
    // for (uint8_t i = 0; i < PCA9965_NUM_LEDS; i++)
    // {
    //     Serial.print(i);
    //     Serial.print(":");
    //     Serial.print(pcaManager.getLEDNo(3, i));
    //     Serial.print(" ");
    // }
    // Serial.println("");
}

void loop()
{
    /*********************** PWM control with sector management function demo *************************************/
    for (uint8_t i = 0; i < 255; i++)
    {

        /******************* Driving sector 0 ******************/
        uint8_t val = (uint8_t)(-cos((float)i / 255 * PI * 2) * 128) + 128;
        uint8_t sectorNo = 0;
        uint8_t deviceNo = pcaManager.getDeviceNo(sectorNo);
        // Serial.print(deviceNo);
        // Serial.print(" ");
        for (uint8_t sectorLEDNo = 0; sectorLEDNo < PCA9965_NUM_LEDS; sectorLEDNo++)
        {
            uint8_t ledNo = pcaManager.getLEDNo(sectorNo, sectorLEDNo);
            // Serial.print(ledNo);
            // Serial.print(" ");
            if (ledNo < PCA9965_NUM_LEDS)
                ledDrivers[deviceNo].pwmLED(ledNo, val);
        }
        // Serial.println("");

        /******************* Driving sector 1 ******************/
        sectorNo = 1;
        deviceNo = pcaManager.getDeviceNo(sectorNo);
        val = (uint8_t)(cos((float)i / 255 * PI * 2) * 128) + 127;
        for (uint8_t sectorLEDNo = 0; sectorLEDNo < PCA9965_NUM_LEDS; sectorLEDNo++)
        {
            uint8_t ledNo = pcaManager.getLEDNo(sectorNo, sectorLEDNo);
            if (ledNo < PCA9965_NUM_LEDS)
                ledDrivers[deviceNo].pwmLED(ledNo, val);
        }

        /******************* Driving sector 2 ******************/
        sectorNo = 2;
        deviceNo = pcaManager.getDeviceNo(sectorNo);
        // Serial.print(deviceNo);
        // Serial.print(" ");
        val = (uint8_t)(cos((float)i / 255 * PI * 2) * 128) + 127;
        for (uint8_t sectorLEDNo = 0; sectorLEDNo < PCA9965_NUM_LEDS; sectorLEDNo++)
        {
            uint8_t ledNo = pcaManager.getLEDNo(sectorNo, sectorLEDNo);
            // Serial.print(ledNo);
            // Serial.print(" ");
            if (ledNo < PCA9965_NUM_LEDS)
                ledDrivers[deviceNo].pwmLED(ledNo, val);
        }
        // Serial.println("");

        /******************* Driving sector 3 ******************/
        sectorNo = 3;
        deviceNo = pcaManager.getDeviceNo(sectorNo);

        val = (uint8_t)(-cos((float)i / 255 * PI * 2) * 8) + 8;

        for (uint8_t sectorLEDNo = 0; sectorLEDNo < PCA9965_NUM_LEDS; sectorLEDNo++)
        {
            uint8_t ledNo = pcaManager.getLEDNo(sectorNo, sectorLEDNo);
            if (ledNo < PCA9965_NUM_LEDS)
                ledDrivers[deviceNo].pwmLED(ledNo, val);
        }

        delay(10);
    }
}
