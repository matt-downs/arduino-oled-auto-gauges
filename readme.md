# Arduino powered OLED gauges for boost and oil pressure

## Completed project

Below is an image showing the completed project installed in the dashboard of my car. Boost pressure gauge is on the left and oil pressure is on the right.

Both gauges show the current value in large easily legible text at the top, and have a scrolling history graph at the bottom. The boost gauge shows the peak boost pressure for the trip at the top right.

All positive values on the graph are filled in, whereas negative values are not. This gives a nice bright emphasis when boost hits a positive pressure :)

![](/example.JPEG)

## Inspiration

A large inspiration for this project was this YouTube video:
![](https://i.ytimg.com/vi/bdnQrl0PDNA/maxresdefault.jpg)
https://www.youtube.com/watch?v=bdnQrl0PDNA

## Hardware

- [Arduino Nano](https://www.ebay.com.au/itm/USB-Nano-V3-0-ATMEGA328P-CH340G-5V-16M-Micro-Controller-Board-Kit-FOR-Arduino/263004104622)
- [1.3" White I2C OLED Display](https://www.ebay.com.au/itm/1-3-Blue-White-IIC-I2C-Serial-128X64-OLED-LCD-Display-Screen-Module-For-Arduino/282658536123)
  - 128x64 resolution
  - SH1106 controller
- Existing Defi analogue boost/oil pressure sensors
  - 5v supply voltage
  - 0.5v -> 4.5v sensor reading
  - Oil pressure sensor range: 140psi (absolute)
  - Boost pressure sensor range: 50psi (absolute)

## Dependencies

- [u8glib](https://github.com/olikraus/u8glib)
