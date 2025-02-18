#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_EPD.h>
#include <Adafruit_SHT4x.h>
#include <RFM69.h>
#include <SPI.h>

// Définir les broches pour l'écran eInk
#define EPD_CS     10
#define EPD_DC      9
#define SRAM_CS     8
#define EPD_RESET   7
#define EPD_BUSY    -1
// Définir les broches pour le module RFM69HCW
#define RF69_FREQ 433.0
#define RF69_CS   6
#define RF69_INT  3
#define RF69_RST  4

Adafruit_SHT4x sht4 = Adafruit_SHT4x();
RFM69 radio(RF69_CS, RF69_INT, true);
// Initialiser l'écran eInk
Adafruit_IL0373 display(128, 250, EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);

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
  Serial.begin(115200);

  // Initialiser l'écran eInk
  display.begin();
  display.clearBuffer();
  // Afficher les icônes et les valeurs
  display.setTextSize(2);
  display.setTextColor(EPD_BLACK);
  
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
  radio.setHighPower();

}

void loop() {
  sensors_event_t humidity, temp;
  sht4.getEvent(&humidity, &temp);

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
  radio.send(1, data, strlen(data));

  // Attendre 5 minutes
  delay(300000);
}
