// Sample code for individual PWM control

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

    // Call reset before init, otherwise init setting will be lost
    ledDrivers[0].resetAllDevices();

    // You can also reset the devices with init(uint8_t devAddress, uint8_t ledBrightness, bool enablePWM = false, bool resetStatus_all = false)
    // The reest flag in init() set false by default
    ledDrivers[0].init(DEV_ADDRESS1, LED_BRIGHTNESS, true);
    ledDrivers[1].init(DEV_ADDRESS2, LED_BRIGHTNESS, true);
    // delay(5000);
}

void loop()
{
    /*********************** simple ON-OFF control demo *************************************/
    uint8_t pattern0[PCA9965_NUM_LEDS];
    uint8_t pattern1[PCA9965_NUM_LEDS];
    for (uint8_t j = 0; j < PCA9965_NUM_LEDS; j++)
    {
        for (uint8_t i = 0; i < PCA9965_NUM_LEDS; i++)
        {
            float val = -cos((float)(i + j) / PCA9965_NUM_LEDS * PI * 2) + 1;
            val *= 128;
            if (val > 255)
                val = 0xFF;
            else if (val < 0)
                val = 0;
            uint8_t val_int = (uint8_t)val;
            // Serial.print(val);
            // Serial.print(" ");

            // Pattern1: You could use PWM9956.pwmLED(uint8_t LEDNo, uint8_t PWMPower), but not efficient,
            //since you are sending command individually
            // ledDrivers[0].pwmLED(i, val_int);
            // ledDrivers[1].pwmLED(PCA9965_NUM_LEDS - i - 1, val_int);

            pattern0[i] = val_int;
            pattern1[PCA9965_NUM_LEDS - i - 1] = val_int;
        }
        // Pattern2: By using PWM9956.setLEDPattern(uint8_t *LEDPattern), you can set PWM values of all 24 leds at once. This is more efficient
        // especially if you have multiple PCA9956s
        ledDrivers[0].setLEDPattern(pattern0);
        ledDrivers[1].setLEDPattern(pattern1);

        // Serial.println();

#ifdef M5ATOM
        if (blink)
            M5.dis.drawpix(0, 0x000000);
        else
            M5.dis.drawpix(0, 0xf00000);
#endif
        blink = !blink;
        delay(20);
    }
}
