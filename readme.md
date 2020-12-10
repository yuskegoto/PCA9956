# PCA9956 Library for Arduino
This is Arduino library for I2C controlled LED driver PCA9956. The chips is capable to handle 24 channels of LEDs, however there is practically **NO** sample code nor information except this rather incomplehensible [datasheet](https://www.nxp.com/docs/en/data-sheet/PCA9956B.pdf). So I hope this code can help someone who are struggling now to understand this guy. I have tested this code with [Switchscience's breakout board](https://www.switch-science.com/catalog/2677/). When all three address setting pins of PCA9956 are open state, the i2c address will be 0x3F. 


[Device datasheet](https://www.switch-science.com/catalog/2677/)

[Product page of Switchscience](https://www.switch-science.com/catalog/2677/)


## Implementation
- Individual On-Off control
- Individual PWM control
- Individual Iref setting
- PWM control with auto INCREMENT feature for faster communication
- Multiple devices and sector management tool

## Procedure

Following is simplified procedure for hooking up the device.

1．Connect the power and i2c wires / i2c及び電源を接続
   
Arduino   PCA9956
  
5V        VDD
  
GND       GND
  
SDA       SDA
  
SCL       SCL

2. Add pull up resistors between Vcc and each i2c wires (SDA / SCL). Please don't forget pull-up regestors!

3. Upload your sketch!


## Resistor Setting

As you can see on the code, I did only on/off and pwm control. Group dimming is also possible, even I haven't implemented yet. The resistor setting precedure for On/Off control is like this.

1. Set up MODE1(Ox00) resistor
   
2. Set up output mode setting LEDOUT0-5(0x02 - 0x7). One resistor has 4 LED's control state. Therefore one LED has two bits of control state... See the datasheet page 17. 
   
    00: LED driver is off
    
    01: LED driver is fully on
    
    10: LED driver can be indivisually controlled from PWMx resistor or PWMALL resistor for all control
        
    11: LED driver's individual brightness and group's dimming/blinking can be controlled from PWMx or GRPPWM resistors
    
3. IREF0 - 23(0x22 - 0x39) out put current control

This procedure is **not written cleary** on the datasheet. So I guess there are bunch of folks giving up before finding this procedure.

Actually I found this procedure from a comment I found on [Amazon.jp's product page](https://www.amazon.co.jp/gp/aw/reviews/B01FJHF760). Thanks! 
