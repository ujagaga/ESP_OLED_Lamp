# ESP OLED Lamp
The purpose is to combine an ESP8266 module and an 1.54" SPI LCD module to create a WiFi controlled clock/lamp 
I used an D1 Mini module. A BC547 transistor is used as a switch.

The device spins up an HTTP server which is tied to the AP, so after the device connects to an external WiFi as station, this web server will not be available on that network, but only on the initial AP.
Spinning a second HTTP web server can be done on same port, but for the second interface, but it takes a lot of RAM, so might crash.