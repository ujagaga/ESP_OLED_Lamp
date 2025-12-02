#ifndef CONFIG_H
#define CONFIG_H

// GPIO definitions
#define TFT_CS   -1    
#define TFT_MOSI 13  
#define TFT_SCLK 14  
#define TFT_DC   5  
#define TFT_RST  4  
#define LED_PIN  15

#define AP_MODE_TIMEOUT_S       (60)              // After this period since startup, try to connect as wifi client.
#define AP_NAME_PREFIX          "SecretSantaClk_" // Will be appended by device MAC
#define AP_PASS                 "hoho1234"

#define REGION                  "Europe/Belgrade" // Required to fetch correct timezone with respect to daylight savings
#define NTP_SYNC_H              (4)               // Sync time every 4 hours

#define WIFI_PASS_EEPROM_ADDR   (0)
#define WIFI_PASS_SIZE          (32)
#define SSID_EEPROM_ADDR        (WIFI_PASS_EEPROM_ADDR + WIFI_PASS_SIZE)
#define SSID_SIZE               (32)
#define TZ_EEPROM_ADDR          (WIFI_PASS_SIZE + SSID_SIZE) 
#define TZ_SIZE                 (64)
#define EEPROM_SIZE             (WIFI_PASS_SIZE + SSID_SIZE + TZ_SIZE)   

#endif
