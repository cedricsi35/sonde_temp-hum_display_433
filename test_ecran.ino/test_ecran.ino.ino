/***************************************************
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "Adafruit_EPD.h"

// #ifdef ARDUINO_ADAFRUIT_FEATHER_RP2040_THINKINK // detects if compiling for
//                                                 // Feather RP2040 ThinkInk
// #define EPD_DC PIN_EPD_DC       // ThinkInk 24-pin connector DC
// #define EPD_CS PIN_EPD_CS       // ThinkInk 24-pin connector CS
// #define EPD_BUSY PIN_EPD_BUSY   // ThinkInk 24-pin connector Busy
// #define SRAM_CS -1              // use onboard RAM
// #define EPD_RESET PIN_EPD_RESET // ThinkInk 24-pin connector Reset
// #define EPD_SPI &SPI1           // secondary SPI for ThinkInk
// #else
#define EPD_DC 9
#define EPD_CS 10
#define EPD_BUSY -1 // can set to -1 to not use a pin (will wait a fixed delay)
#define SRAM_CS 8
#define EPD_RESET 7  // can set to -1 and share with microcontroller Reset!
#define EPD_SPI &SPI // primary SPI
// #endif



// Uncomment the following line if you are using 2.13" EPD with SSD1680
Adafruit_SSD1680 display(250, 122, EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);


#define COLOR1 EPD_BLACK
#define COLOR2 EPD_RED

void setup() {
  Serial.begin(115200);
  // while (!Serial) { delay(10); }
  Serial.println("begin");
  display.begin();
  delay(5000);

  // large block of text
  Serial.println("clearBuffer");
  display.clearBuffer();
    delay(5000);


  Serial.println("drawtext");
  testdrawtext(
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur "
      "adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, "
      "fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor "
      "neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet "
      "ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a "
      "tortor imperdiet posuere. ",
      COLOR1);
    delay(5000);


  Serial.println("display.display");
  display.display(true);


  delay(5000);

  display.clearBuffer();
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(0, 0, i, display.height() - 1, COLOR1);
  }

  for (int16_t i = 0; i < display.height(); i += 4) {
    display.drawLine(display.width() - 1, 0, 0, i,
                     COLOR2); // on grayscale this will be mid-gray
  }
  display.display();
}

void loop() {
  // don't do anything!
}

void testdrawtext(const char *text, uint16_t color) {
  display.setCursor(0, 0);
  display.setTextColor(color);
  display.setTextWrap(true);
  display.print(text);
}
