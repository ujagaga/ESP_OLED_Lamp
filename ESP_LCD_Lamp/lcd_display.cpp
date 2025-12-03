#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "config.h"
#include "lcd_display.h"

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void drawSnowflake(int x, int y, int r, uint16_t color) {
  const float branchLen = 0.4;      // how long the side branches are (0.4 * r)
  const float branchOffset1 = 0.55; // location along arm (55%)
  const float branchOffset2 = 0.8;  // second location (80%)

  for (int i = 0; i < 6; i++) {
    float ang = i * 60 * DEG_TO_RAD;

    // main arm
    int x2 = x + r * cos(ang);
    int y2 = y + r * sin(ang);
    tft.drawLine(x, y, x2, y2, color);

    // two branch positions on each arm
    for (float k : {branchOffset1, branchOffset2}) {
      int bx = x + (r * k) * cos(ang);
      int by = y + (r * k) * sin(ang);

      // +30° branch
      float ang1 = ang + 30 * DEG_TO_RAD;
      int b1x = bx + (r * branchLen) * cos(ang1);
      int b1y = by + (r * branchLen) * sin(ang1);

      // -30° branch
      float ang2 = ang - 30 * DEG_TO_RAD;
      int b2x = bx + (r * branchLen) * cos(ang2);
      int b2y = by + (r * branchLen) * sin(ang2);

      tft.drawLine(bx, by, b1x, b1y, color);
      tft.drawLine(bx, by, b2x, b2y, color);
    }
  }
}

void LCD_init() 
{
  SPI.begin(); // default hardware SPI pins, no need to pass pins
  tft.init(240, 240, SPI_MODE3);

  tft.fillScreen(C_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(C_BLUE);
  tft.setTextSize(3);
  tft.println(" Secret Santa\n");
  tft.setTextColor(C_GREEN);
  tft.setTextSize(8);
  tft.println("Clock");

  drawSnowflake(40, 180, 25, C_WHITE);
  drawSnowflake(90, 140, 25, C_WHITE);
  drawSnowflake(140, 170, 25, C_WHITE);
  drawSnowflake(190, 130, 25, C_WHITE);
}

void LCD_clear()
{
  tft.fillScreen(C_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(C_WHITE);
  tft.setTextSize(2);
}

void LCD_textSize(int txtSize)
{
  tft.setTextSize(txtSize);
}

void LCD_color(uint16 c)
{
  tft.setTextColor(c);
}

void LCD_write(String msg)
{
  tft.print(msg);
}
