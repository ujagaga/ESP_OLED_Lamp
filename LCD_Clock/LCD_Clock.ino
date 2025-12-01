#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// Pins (use the D# names so it's clear)
#define TFT_CS    -1      // module tied low or not present
#define TFT_DC     D1     // Data/Command -> D1 (GPIO5)
#define TFT_RST    D0     // Reset -> D0 (GPIO16)

// #define TFT_MOSI 13  // D7
// #define TFT_SCLK 14  // D5
// #define TFT_DC    5  // D1
// #define TFT_RST  16  // D0

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  // hardware SPI on ESP8266 uses SCK = D5 (GPIO14), MOSI = D7 (GPIO13)
  SPI.begin(); // default hardware SPI pins, no need to pass pins
  tft.init(240, 240); // set to your panel resolution
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(10, 10);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.println("Hello ST7789");
}

void loop() {
  // demo
  delay(1000);
}
