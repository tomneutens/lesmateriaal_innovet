#include <Wire.h>

#include <Dwenguino.h>

#include <LiquidCrystal.h>

void setup()
{
  initDwenguino();
  pinMode(SW_S, INPUT_PULLUP);
  while (digitalRead(SW_S) == HIGH){
    // Doe niets, wacht tot knop wordt ingedrukt.
  }
}

void loop()
{
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
}