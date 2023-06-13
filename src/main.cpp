#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_CS 15
#define TFT_RST 5
#define TFT_DC 22
#define TFT_SCLK 18
#define TFT_MOSI 23


Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);

  tft.begin(40000000);

  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.fillScreen(ILI9341_BLACK);  // Set the background color to black
  tft.println("Du nervst");
}

void loop() {
  // Nothing to do here
}