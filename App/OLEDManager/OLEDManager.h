#ifndef OLEDMANAGER_H
#define OLEDMANAGER_H

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#define LCD_MOSI 23
#define LCD_SCLK 18
#define LCD_CS   15
#define LCD_DC   2
#define LCD_RST  4
#define LCD_BLK  32

extern Adafruit_ST7789 tft;

void initOLED();
void displayMessage(String line1, String line2);

#endif
