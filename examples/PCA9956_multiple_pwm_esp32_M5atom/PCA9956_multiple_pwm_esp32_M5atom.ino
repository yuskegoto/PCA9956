#include <Arduino.h>
#include <Wire.h>

#include "PCA9956.h"
#include "Setting.h"

#define M5ATOM

#ifdef M5ATOM
#include "M5Atom.h"
#endif

PCA9956 ledDrivers[2]{{&Wire}, {&Wire}}; // if you have multiple drivers, init this way...
// PCA9956 ledDrivers[9]{{&Wire}, {&Wire}, {&Wire}, {&Wire}, {&Wire}, {&Wire}, {&Wire}, {&Wire}, {&Wire}}; // if you have multiple drivers, init this way...

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
  // ledDrivers[2].init(DEV_ADDRESS3, LED_BRIGHTNESS);
  // ledDrivers[3].init(DEV_ADDRESS4, LED_BRIGHTNESS);
  // ledDrivers[4].init(DEV_ADDRESS5, LED_BRIGHTNESS);
  // ledDrivers[5].init(DEV_ADDRESS6, LED_BRIGHTNESS);
  // ledDrivers[6].init(DEV_ADDRESS7, LED_BRIGHTNESS);
  // ledDrivers[7].init(DEV_ADDRESS8, LED_BRIGHTNESS);
  // ledDrivers[8].init(DEV_ADDRESS9, LED_BRIGHTNESS);
}

void loop()
{
  /*********************** PWM control demo *************************************/
  uint8_t pattern[PCA9965_NUM_LEDS];
  for (uint8_t i = 0; i < 255; i++)
  {
    uint8_t val = (uint8_t)(-cos((float)i / 255 * PI * 2) * 128) + 128;
    for (uint8_t j = 0; j < PCA9965_NUM_LEDS; j++)
    {
      pattern[j] = val;
    }
    ledDrivers[0].setLEDPattern(pattern);
    val = (uint8_t)(cos((float)i / 255 * PI * 2) * 128) + 128;
    for (uint8_t j = 0; j < PCA9965_NUM_LEDS; j++)
    {
      pattern[j] = val;
    }

    ledDrivers[1].setLEDPattern(pattern);
    delay(10);
  }

  /*********************** simple ON-OFF control demo *************************************/
  for (uint8_t i = 0; i < PCA9965_NUM_LEDS; i++)
  {
    ledDrivers[0].onLED(i);
    ledDrivers[1].onLED(PCA9965_NUM_LEDS - i - 1);
    // ledDrivers[2].onLED(i);
    // ledDrivers[3].onLED(i);
    // ledDrivers[4].onLED(i);
    // ledDrivers[5].onLED(i);
    // ledDrivers[6].onLED(i);
    // ledDrivers[7].onLED(i);
    // ledDrivers[8].onLED(i);

#ifdef M5ATOM
    if (blink)
      M5.dis.drawpix(0, 0x000000);
    else
      M5.dis.drawpix(0, 0xf00000);
    blink = !blink;
#endif

    delay(500);

    ledDrivers[0].offLED(i);
    ledDrivers[1].offLED(PCA9965_NUM_LEDS - i - 1);
    // ledDrivers[2].offLED(i);
    // ledDrivers[3].offLED(i);
    // ledDrivers[4].offLED(i);
    // ledDrivers[5].offLED(i);
    // ledDrivers[6].offLED(i);
    // ledDrivers[7].offLED(i);
    // ledDrivers[8].offLED(i);
  }
}
