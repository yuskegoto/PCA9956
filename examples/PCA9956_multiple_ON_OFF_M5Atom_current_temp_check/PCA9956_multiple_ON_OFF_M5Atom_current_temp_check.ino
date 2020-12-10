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

void setup()
{
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
}

void loop()
{
    /*********************** simple ON-OFF control demo *************************************/
    for (uint8_t i = 0; i < PCA9965_NUM_LEDS; i++)
    {
        ledDrivers[0].onLED(i);
        ledDrivers[1].onLED(PCA9965_NUM_LEDS - i - 1);
#ifdef M5ATOM
        if (blink)
            M5.dis.drawpix(0, 0x000000);
        else
            M5.dis.drawpix(0, 0xf00000);
        blink = !blink;
#endif
        delay(200);
    }
    delay(1000);

    for (uint8_t i = 0; i < PCA9965_NUM_LEDS; i++)
    {
        ledDrivers[0].offLED(i);
        ledDrivers[1].offLED(PCA9965_NUM_LEDS - i - 1);
        delay(200);
    }

    if (ledDrivers[0].checkTempWarning())
        Serial.println("Temp warning");
    for (uint8_t i = 0; i < 6; i++)
    {
        uint8_t errorStatus = ledDrivers[0].getLEDErrorStatus(ERROR_LED0_3 + i);
        // Serial.println(errorStatus, BIN);
        if (errorStatus > 0)
        {
            Serial.println("LED error");
            if (errorStatus & ERROR_OPEN_CIRCUIT)
                Serial.println("open circuit");
            if (errorStatus & ERROR_SHORT_CIRCUIT)
                Serial.println("short circuit");
        }
    }
    delay(1000);
}
