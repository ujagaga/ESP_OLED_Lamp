#ifndef LCDDISPLAY_H
#define LCDDISPLAY_H

#define C_BLACK 0x0000
#define C_WHITE 0xFFFF
#define C_RED 0xF800
#define C_GREEN 0x07E0
#define C_BLUE 0x001F
#define C_CYAN 0x07FF
#define C_MAGENTA 0xF81F
#define C_YELLOW 0xFFE0
#define C_ORANGE 0xFC00

extern void LCD_init(void);
extern void LCD_clear(void);
extern void LCD_textSize(int txtSize);
extern void LCD_color(uint16 c);
extern void LCD_write(String msg);

#endif
