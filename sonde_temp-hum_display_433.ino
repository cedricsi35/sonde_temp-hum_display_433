#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_EPD.h>
#include <Adafruit_SHT4x.h>
#include <RFM69.h>
#include <SPI.h>
//#include <LowPower.h>
#include <RFM69_ATC.h>
#include "Adafruit_ThinkInk.h"

// Définir les broches pour l'écran eInk
#define EPD_DC 10
#define EPD_CS 9
#define EPD_BUSY 7 // can set to -1 to not use a pin (will wait a fixed delay)
#define SRAM_CS 6
#define EPD_RESET 8  // can set to -1 and share with microcontroller Reset!
#define EPD_SPI &SPI // primary SPI

// Définir les broches pour le module RFM69HCW
#define IS_RFM69HW_HCW
#define FREQUENCY RF69_433MHZ
#define RF69_FREQ 433.0
#define RF69_CS   5
#define RF69_INT  3
#define RF69_RST  4


Adafruit_SHT4x sht4 = Adafruit_SHT4x();
RFM69 radio(RF69_CS, RF69_INT, true);
// Initialiser l'écran eInk
//Adafruit_IL0373 display(128, 250, EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);
ThinkInk_213_Tricolor_RW display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY,EPD_SPI);

// Définir les icônes (exemples simples)
const unsigned char temperatureIcon[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char humidityIcon[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void setup() {
pinMode(10,OUTPUT);
pinMode(9,OUTPUT);
pinMode(7,INPUT);
pinMode(6,OUTPUT);
pinMode(8,OUTPUT);
pinMode(5,OUTPUT);
pinMode(3,OUTPUT);
pinMode(4,OUTPUT);
  Serial.begin(115200);

  // Initialiser l'écran eInk
  display.begin(THINKINK_TRICOLOR);
  display.clearBuffer();
  // Afficher les icônes et les valeurs
   
  // Afficher un message de démarrage
  display.setTextSize(2);
  display.setTextColor(EPD_BLACK);
  display.setCursor(10, 50);
  display.println("Initialisation...");
  display.display();
  delay(2000);

  // Afficher l'icône de température
  display.drawBitmap(10, 10, temperatureIcon, 16, 16, EPD_BLACK);
  display.setCursor(30, 10);
  display.print("Temp: 25.0 C");
  // Afficher l'icône d'humidité
  display.drawBitmap(10, 40, humidityIcon, 16, 16, EPD_BLACK);
  display.setCursor(30, 40);
  display.print("Humidity: 60%");
  // Mettre à jour l'affichage
  display.display();

    // Initialiser le capteur SHT45
  if (!sht4.begin()) {
    Serial.println("Couldn't find SHT4x");
    while (1) delay(1);
  }

   // Initialiser le module RFM69HCW
  radio.initialize(RF69_FREQ, 1, 100);
  //radio.setHighPower();

  radio.sleep();

}

void loop() {
  Serial.println("send radio");
  
  sensors_event_t humidity, temp;
  sht4.getEvent(&humidity, &temp);
  Serial.println(temp.temperature);
  Serial.println(humidity.relative_humidity);
  // Afficher les données sur l'écran eInk
  display.clearBuffer();
  display.setTextSize(2);
  display.setTextColor(EPD_BLACK);
  display.setCursor(10, 50);
  display.print("Temp: ");
  display.print(temp.temperature);
  display.println(" C");
  display.setCursor(10, 80);
  display.print("Humidity: ");
  display.print(humidity.relative_humidity);
  display.println(" %");
  display.display();

  // Transmettre les données via le module RFM69HCW

  char data[50];
sprintf(data, "Temp: %.2f C, Humidity: %.2f %%", temp.temperature, humidity.relative_humidity);
 // sprintf(data, "Temp: %.2f C, Humidity: %.2f %%", 1848, 1900);
  Serial.println(data);
  Serial.println(strlen(data));

//  radio.send(1,"12345678912345678901234567890azertyuiop" , 40);
  radio.send(1, data, 10);
Serial.println("data sent");
//radio.sleep();
  // Attendre 5 minutes
  delay(50000);
}
