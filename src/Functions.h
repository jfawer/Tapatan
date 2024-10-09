#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Definiere die Custom Characters mit uint8_t statt byte
uint8_t verticalLine[] = {
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100
};

uint8_t horizontalLine[] = {
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111
};

// Funktionsdefinition direkt in der Header-Datei
void drawBoard(LiquidCrystal_I2C &lcd) {
  // Lade die benutzerdefinierten Zeichen ins LCD
  lcd.createChar(0, verticalLine);
  lcd.createChar(1, horizontalLine);
  
  // Lösche das Display
  lcd.clear();
  
  // Zeichne das Tic-Tac-Toe-Spielfeld
  lcd.setCursor(6, 0);
  lcd.write(uint8_t(0)); // Vertikale Linie
  
  lcd.setCursor(13, 0);
  lcd.write(uint8_t(0)); // Vertikale Linie
  
  lcd.setCursor(0, 1);
  lcd.write(uint8_t(1)); // Horizontale Linie
  
  lcd.setCursor(0, 2);
  lcd.write(uint8_t(1)); // Horizontale Linie
}

#endif
