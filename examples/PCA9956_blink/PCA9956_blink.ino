// Please always refer PCA9956 datasheet, I know the datasheets, especially this one isn't always friendly, but this is the only one documentation we can rely on: https://www.nxp.com/docs/en/data-sheet/PCA9956B.pdf
// Sample sketch for blinking using GRPFREQ and GRPPWM register

// Author: Yuske Goto
// Mar.06.2022

#include <Arduino.h>
#include <Wire.h>

#include "PCA9956.h"
#define LED_BRIGHTNESS 0x06

PCA9956 ledDriver(&Wire);

void setup()
{
  // Serial.begin(9600);

  Wire.begin(21, 22);

  ledDriver.init(0x3D, LED_BRIGHTNESS, false, true);
  ledDriver.setBlinking(0x0D); // 0x0D to blink almost 1Hz
}

void loop()
{
  digitalWrite(5, HIGH);
}
