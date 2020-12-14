// Code sample for individual current control with pwm
// The difference to the PCA9956_multiple_pwm_individualControl_esp32 is
// that this code controls different PWM values on each Pins at the same time
// while the code mentioned above sends same value to all LED pins.

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
            ledDrivers[0].pwmLED(i, val_int);
            ledDrivers[1].pwmLED(PCA9965_NUM_LEDS - i - 1, val_int);
        }
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
