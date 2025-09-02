#include "OLEDManager.h"

Adafruit_ST7789 tft = Adafruit_ST7789(LCD_CS, LCD_DC, LCD_RST);

void initOLED() {
  pinMode(LCD_BLK, OUTPUT);
  digitalWrite(LCD_BLK, HIGH);
  tft.init(170, 320, SPI_MODE2);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextWrap(true);
}

void displayMessage(String line1, String line2) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 40);
  tft.println(line1);
  tft.setCursor(10, 80);
  tft.println(line2);
}
