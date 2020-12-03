#include <Arduino.h>
#include <Wire.h>

#include "PCA9956.h"

// i2c addresses
#define DEV_ADDRESS1 0x3D
#define DEV_ADDRESS2 0x3F

// led control settings
#define LED_BRIGHTNESS 0x06

//#define M5ATOM

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
  M5.begin(true, false, true);
  delay(50);
  M5.dis.drawpix(0, 0x000000);
#else
  Serial.begin(9600);
#endif

  Wire.begin(21, 22); // Wire needs to init separately...

  ledDrivers[0].init(DEV_ADDRESS2, LED_BRIGHTNESS);
  ledDrivers[1].init(DEV_ADDRESS1, LED_BRIGHTNESS);
}

void loop()
{
    /*********************** PWM control demo *************************************/
    for (uint8_t i = 0; i < 255; i++)
    {
        uint8_t val = (uint8_t) (-cos((float)i/255 * PI * 2) * 128) + 128;
        for (uint8_t j = 0; j < PCA9965_NUM_LEDS; j++)
        {
            ledDrivers[0].pwmLED(j, val);
        }

        val = (uint8_t) (cos((float)i / 255 * PI * 2) * 128) + 128;
        for (uint8_t j = 0; j < PCA9965_NUM_LEDS; j++)
        {
            ledDrivers[1].pwmLED(j, val);
        }

        delay(10);
    }
}
