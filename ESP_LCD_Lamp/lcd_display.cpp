#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "config.h"
#include "lcd_display.h"

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void LCD_init() 
{
  SPI.begin(); // default hardware SPI pins, no need to pass pins
  tft.init(284, 76, SPI_MODE1);

  tft.fillScreen(C_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(C_BLUE);
  tft.setTextSize(3);
  tft.println(" Ivanin sat\n");
  tft.setTextColor(C_GREEN);  
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
