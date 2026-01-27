#include "ST7789_Custom.h"
#include "lcd_display.h"
#include <Fonts/FreeMonoBold9pt7b.h> 
#include <Fonts/FreeMonoBold12pt7b.h> 
#include <Fonts/FreeMonoBold18pt7b.h> 
#include <Fonts/FreeMonoBold24pt7b.h> 
#include <Fonts/TomThumb.h>

ST7789_Custom tft;

static uint16_t bgColor = C_BLACK; 
static uint16_t fgColor = C_YELLOW; 
static bool invertedColors = false;

static uint16_t invertColor(uint16_t color) {
  return ~color;
}

void LCD_init() 
{
  tft.begin();
  // Set to landscape for a wide "bar" look. Use 0 or 2 for portrait.
  tft.setRotation(1); 
  tft.fillScreen(bgColor);
  tft.setCursor(0, 0);
  tft.setTextSize(1);
}

void LCD_setFont(FontStyle id){
  if(id == Font24pt){
    tft.setFont(&FreeMonoBold24pt7b);
  }else if(id == Font18pt){
    tft.setFont(&FreeMonoBold18pt7b);
  }else if(id == Font12pt){
    tft.setFont(&FreeMonoBold12pt7b);
  }else{
    tft.setFont(&FreeMonoBold9pt7b); 
  }
}

void LCD_clear()
{
  tft.fillScreen(bgColor);
  tft.setCursor(0, 0);
  tft.setTextColor(fgColor);
  tft.setTextSize(1);
  LCD_setFont(Font9pt);
}

void LCD_setX(int x){
  int y = tft.getCursorY();
  tft.setCursor(x, y);
}

int LCD_getX(void){
  return tft.getCursorX();
}

void LCD_textSize(int txtSize)
{
  tft.setTextSize(txtSize);
}

void LCD_color(uint16 c)
{
  tft.setTextColor(c);
}

uint16_t LCD_getBgColor(void){
  return bgColor;
}

void LCD_setBgColor(uint16_t color){
  bgColor = color;
}

uint16_t LCD_getFgColor(void){
  return fgColor;
}

void LCD_setFgColor(uint16_t color){
  fgColor = color;
}

// NOTE: FreeType fonts draw on top of base line, so at coordinates 0,0 the text starts outside the screen.
// First set font size and write a new line character. This will set the cursor at the right place for first row.
void LCD_write(String msg)
{
  tft.print(msg);
}

void LCD_clearStringArea(String msg) {
  int16_t x1, y1;
  uint16_t w, h;
  
  // This calculates the bounding box of the string
  tft.getTextBounds(msg, tft.getCursorX(), tft.getCursorY(), &x1, &y1, &w, &h);
  
  // Fill that box with black
  tft.fillRect(x1, y1, w, h, C_BLACK);
}

void LCD_setInverted(bool inverted){
  if(invertedColors != inverted){
    invertedColors = inverted;
    bgColor = invertColor(bgColor);
    fgColor = invertColor(fgColor);
  }
}
