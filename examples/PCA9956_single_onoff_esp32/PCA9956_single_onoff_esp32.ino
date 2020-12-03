#include <Arduino.h>
#include <Wire.h>

#include "PCA9956.h"
#define LED_BRIGHTNESS 0x06

PCA9956 ledDriver(&Wire);

void setup()
{
  Serial.begin(9600);

  Wire.begin(21, 22); // Wire needs to init separately...

  ledDriver.init(0x3F, LED_BRIGHTNESS);
}

void loop()
{

  /*********************** simple ON-OFF control demo *************************************/
  for (uint8_t i = 0; i < PCA9965_NUM_LEDS; i++)
  {
    ledDriver.onLED(i);

    delay(500);

    ledDriver.offLED(i);
  }
}
