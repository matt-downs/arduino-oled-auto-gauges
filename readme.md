# Arduino powered OLED gauges for boost and oil pressure

## Inspiration
![](https://i.ytimg.com/vi/bdnQrl0PDNA/maxresdefault.jpg)
https://www.youtube.com/watch?v=bdnQrl0PDNA

## Hardware
- [Arduino Nano](https://www.ebay.com.au/itm/USB-Nano-V3-0-ATMEGA328P-CH340G-5V-16M-Micro-Controller-Board-Kit-FOR-Arduino/263004104622)
- [1.3" White I2C OLED Display](https://www.ebay.com.au/itm/1-3-Blue-White-IIC-I2C-Serial-128X64-OLED-LCD-Display-Screen-Module-For-Arduino/282658536123)
    - 128x64 resolution
    - SH1106 controller - should be able to use [this](https://github.com/ThingPulse/esp8266-oled-ssd1306) library
- Existing Defi analogue boost/oil pressure sensors
    - 5v supply voltage