#include "config.h"

#ifdef USE_ADAFRUIT_ST7789
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

Adafruit_ST7789 tft = Adafruit_ST7789(-1, TFT_DC, TFT_RST);
#else
#include "ST7789_Custom.h"

ST7789_Custom tft;
#endif

#include <Fonts/FreeMonoBold9pt7b.h> 
#include <Fonts/FreeMonoBold12pt7b.h> 
#include <Fonts/FreeMonoBold18pt7b.h> 
#include <Fonts/FreeMonoBold24pt7b.h> 
#include <Fonts/TomThumb.h>

#include "lcd_display.h"


static uint16_t bgColor = C_BLACK; 
static uint16_t fgColor = C_YELLOW; 
static bool invertedColors = false;

static uint16_t invertColor(uint16_t color) {
  return ~color;
}

#ifdef USE_ADAFRUIT_ST7789
void LCD_init() 
{
  SPI.begin(); // default hardware SPI pins, no need to pass pins
  tft.init(SCREEN_W, SCREEN_H, SPI_MODE3);
  tft.setRotation(0); 

  tft.fillScreen(bgColor);
  tft.setCursor(0, 0);  
  tft.setTextSize(1);
  tft.setTextColor(fgColor);
}
#else
void LCD_init() 
{
  tft.begin();
  // Set to landscape for a wide "bar" look. Use 0 or 2 for portrait.
  tft.setRotation(1); 

  tft.fillScreen(bgColor);
  tft.setCursor(0, 0);
  tft.setTextSize(1);
  tft.setTextColor(fgColor);

}
#endif

void LCD_drawSnowflake(int x, int y, int r, uint16_t color) {
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
