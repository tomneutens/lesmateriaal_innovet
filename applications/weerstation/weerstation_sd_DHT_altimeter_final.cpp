
// Bibliotheken inladen
#include <LiquidCrystal.h>
#include <Wire.h>
#include <dht.h>    
#include <Adafruit_MPL3115A2.h>
#include <Dwenguino.h>
#include <OneWire.h>
#include <SPI.h>
#include "SD.h"


// Stel in met welke digitale pin de DHT module verbonden is.
#define DHT11PIN 3 
dht DHT; 

// Initialiseer de luchtdrukmeter.
Adafruit_MPL3115A2 mpl;

// Stel de pinnummers in die op de Dwenguino gebruikt worden voor SPI communicatie.
const int DW_CS = 10;
const int DW_MOSI = 2;
const int DW_MISO = 12;
const int DW_CLK = 13;

// Maak een bestand aan 
File dataFile;

void setup()
{
  initDwenguino(); // Initialiseer de basisfuncties van de Dwenguino


  // Wacht om te starten tot de gebruiker op de S(outh) knop drukt.
  dwenguinoLCD.clear();
  dwenguinoLCD.print("Druk op S-knop");
  while(digitalRead(SW_S)){
    ;
  }
  
  // SD kaart verbinding klaarmaken
  dwenguinoLCD.clear();
  dwenguinoLCD.print("SD-kaart setup.");

  // see if the SD card is present and can be initialized:
  if (!SD.begin(DW_CS, DW_MOSI, DW_MISO, DW_CLK)) {
    dwenguinoLCD.clear();
    dwenguinoLCD.print("Geen SD-kaart?");
    // don't do anything more:
    return;
  }
  dwenguinoLCD.clear();
  dwenguinoLCD.print("Kaart klaar");

  /*
    Maak een bestand op de SD-kaart waar je gegevens naar
    gaat schrijven. LET OP!! De naam van het bestand mag maar
    acht tekens lang zijn bv. 12345678.txt.
  */
  dataFile = SD.open("weer.txt", FILE_WRITE);
  if (! dataFile) {
    dwenguinoLCD.clear();
    dwenguinoLCD.print("Fout bij openen.");
    // Wait forever since we cant write data
    while (1) ;
  }

  // Test connectie met luchtdrukmeter
  if (!mpl.begin()) {
    dwenguinoLCD.clear();
    dwenguinoLCD.print("ERROR luchtdruk");
    while(1);
  }

  // Stel in dat de sensor luchtdruk moet meten.
  mpl.setMode(MPL3115A2_BAROMETER);

  // Wacht 1s voor te starten met de hoofdlus.
  delay(1000);

  // Schrijf een header naar het bestand
  String data_header = "Luchtdruk (hPa);Temperatuur (°C);Luchtvochtigheid (%)";
  dataFile.println(data_header);
}

void loop()
{
  // Doe metingen zolang de N(orth) knop niet wordt ingedrukt.
  if (digitalRead(SW_N)){

    // Toon de data op het scherm.
    dwenguinoLCD.clear();
    dwenguinoLCD.print("Start meting");

    // Laat de DHT sensor een meting doen.    
    int chk = DHT.read11(DHT11PIN);

    // Laat de luchtdrukmeter een meting doen
    mpl.startOneShot();
    while (!mpl.conversionComplete()) {
     ;
    }


    // Voeg temperatuur en vochtigheidsgraad samen in csv formaat.
    String data_point = String(mpl.getLastConversionResults(MPL3115A2_PRESSURE))
                          + ";"
                          + String(DHT.temperature)
                          + ";"
                          + String(DHT.humidity);

    // Toon de data op het scherm.
    dwenguinoLCD.clear();
    dwenguinoLCD.print(data_point);

    // Schrijf de data naar het bestand.
    dataFile.println(data_point);

    // Wacht 1s voor je een volgende meting doet.
    delay(1000);

  } else { // Sluit het bestand wanneer de N(orth) knop is ingedrukt.
    dwenguinoLCD.clear();
    dwenguinoLCD.print("closing file");

    // Schrijf de data weg naar het bestand en sluit het bestand.
    dataFile.flush();
    dataFile.close();
    delay(10000);
  }
}