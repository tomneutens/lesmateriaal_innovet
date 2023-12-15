#include <Dwenguino.h>
#include <LiquidCrystal.h>

int voltageBatterij = 0;

void setup(){
  initDwenguino();
  pinMode(A0, INPUT);
  LEDS = 0xff;
  delay(4000);
}

void loop(){
  voltageBatterij = analogRead(A5);
  voltageBatterij = voltageBatterij >> 7; // deel door 128
  dwenguinoLCD.print("Voltage: " + String(voltageBatterij)); // Show text on the display.
  LEDS = (1 << voltageBatterij) - 1; // Zet leds aan 
  delay(50);
  LEDS = 0x00;
  dwenguinoLCD.clear();
}