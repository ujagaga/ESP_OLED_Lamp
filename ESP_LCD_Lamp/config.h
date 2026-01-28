#ifndef CONFIG_H
#define CONFIG_H

// Uncomment to use display via Adafruit driver (good for 240x240), Otherwise a custom driver will be used.
// If you use the custom driver, do not forget to adjust offsets in "ST7789_Custom.h"
// #define USE_ADAFRUIT_ST7789

#define SCREEN_W  76
#define SCREEN_H  284

#define AP_NAME_PREFIX          "LcdClk_"         // Will be appended by device MAC
#define AP_PASS                 "pass1234"

#define WIFI_PASS_EEPROM_ADDR   (0)
#define WIFI_PASS_SIZE          (32)
#define SSID_EEPROM_ADDR        (WIFI_PASS_EEPROM_ADDR + WIFI_PASS_SIZE)
#define SSID_SIZE               (32)
#define EEPROM_SIZE             (WIFI_PASS_SIZE + SSID_SIZE)   

#define TFT_DC    5
#define TFT_RST   4

#endif
