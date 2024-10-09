#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Functions.h"

// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  // Initialize the LCD
  lcd.begin(20, 4);
  // Turn on the backlight
  lcd.backlight();
  
  // Draw the Tic-Tac-Toe board
  drawBoard(lcd);
}

void loop() {
  // Hier kannst du Code hinzufügen, um das Display zu aktualisieren
}
