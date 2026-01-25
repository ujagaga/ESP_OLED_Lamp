#include "ST7789_Custom.h"

ST7789_Custom tft;

void setup() {
  tft.begin();
  tft.setRotation(1); // Set to landscape for a wide "bar" look
  tft.fillScreen(0x0000);
  
  tft.setCursor(20, 30);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);
  tft.println("SYSTEM READY");
}

void loop() {}