# ESP OLED Lamp
The purpose is to combine an ESP8266 module and an 1.54" SPI LCD module to create a WiFi controlled clock/lamp 
I used an D1 Mini module. A BC547 transistor is used as a switch to power the LEDs.

The device spins up an HTTP server which is tied to the AP, so after the device connects to an external WiFi as station, this web server might not be available on that network, but only on the initial AP.
Spinning a second HTTP web server can be done on same port, but for the second interface, but it takes a lot of RAM, so might crash.

On startup the display shows AP SSID: "SecretSantaClk_<dev_mac>" and PASS: "hoho1234". You can connect to this AP using your phone and configure which WiFi to connect to to get internet and tell actual time.

## Further improvements

Here are some ideas:

1. Change colours to your preference. The file "lcd_display.h" contains some predefined colors, but you could add your own.
2. Add date display.
3. Add weather forcast. There are some free API's you could use to get weather for a specific location.
