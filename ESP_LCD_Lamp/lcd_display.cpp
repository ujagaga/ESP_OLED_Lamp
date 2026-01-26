#include "ST7789_Custom.h"
#include "lcd_display.h"


ST7789_Custom tft;

void LCD_init() 
{
  tft.begin();
  tft.setRotation(1); // Set to landscape for a wide "bar" look

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
