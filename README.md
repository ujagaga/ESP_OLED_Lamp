# ESP OLED Lamp
The purpose is to combine an ESP8266 module and an SPI LCD module to create a WiFi controlled clock/lamp 
I used an D1 Mini as WiFi module ans a BC547 transistor as a switch.

This main git branch contains the code I used for an LCD with resolution 76x284px and is intended to be placed horizontally. There is another git branch "tft240x240" intended for an LCD with resolution 240x240px. 
The main difference, beside resolution is that the 240x240 screen worked well with Adafruit library, but for this "weird" resolution LCD, I had to write a custom driver as none I tried worked reliably.

The device spins up an HTTP server which is tied to the AP, so after the device connects to an external WiFi as station, this web server will not be available on that network, but only on the initial AP.
Spinning a second HTTP web server can be done on same port, but for the second interface, but it takes a lot of RAM, so might crash.

## Further improvements

Here are some ideas:

1. Change colours to your preference. The file "lcd_display.h" contains some predefined colors, but you could add your own.
2. Add date display.
3. Add weather forcast. There are some free API's you could use to get weather for a specific location.
