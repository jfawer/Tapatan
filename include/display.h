// Funktionen zur Anzeige von Texten auf dem LCD-Display und zur Eingabe von Spielvariablen

#ifndef DISPLAY_H
#define DISPLAY_H

// Einbinden der Bibliotheken
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Einbinden der Header-Dateien
#include "struct.h"

// Funktion für die Anzeige des Startbildschirms
void displayStart(LiquidCrystal_I2C lcd) {
  lcd.clear();                                                                                                    // LCD löschen
  lcd.setCursor(0, 0);
  lcd.print("Tapatan und");                                                                                       // Text auf dem LCD anzeigen       
  lcd.setCursor(0, 1);
  lcd.print("Tic Tac Toe");
}

void displayReset(LiquidCrystal_I2C lcd) {
  lcd.clear();                                                                                                    // LCD löschen
  lcd.setCursor(0, 0);
  lcd.print("Zuruecksetzen");                                                                                     // Text auf dem LCD anzeigen       
  lcd.setCursor(0, 1);
  lcd.print("des Spielfelds");
}
// Funktion zum Zeichnen Spielfelds
void displayGameBoard(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board[3][3], int currentPlayer) {
  lcd.clear();                                                                                                    // LCD löschen

  lcd.setCursor(0, 0);
  lcd.print(gameSettings.game == 1 ? "Tic Tac Toe:" : "Tapatan:");                                                // Zeige den Namen des Spiels

  lcd.setCursor(0, 2);
  lcd.print("Am Zug:");                                                                                           // Zeige den Spieler, der am Zug ist               
  lcd.setCursor(0, 3);                                                                                    
  if (gameSettings.mode == 1) {
    lcd.print(currentPlayer == 1 ? "Spieler" : "Computer");
  } else {
    lcd.print(currentPlayer == 1 ? "Spieler 1" : "Spieler 2");
  }
  
  for (int row = 0; row < 3; row++) {                                                                             // Zeichne das Spielfeld
    for (int col = 0; col < 3; col++) {
      lcd.setCursor(col + 15, row + 1);
      
      
      char symbol = (Board[row][col] == 0) ? '_' : (Board[row][col] == 1) ? 'X' : 'O';                            // Entscheide, welches Symbol angezeigt werden soll
      lcd.print(symbol);
    }
  }
}

// Funktion zur Anzeige zum Zurücklegen einer unerlaubten Bewegung
void displayIllegalMove(LiquidCrystal_I2C &lcd) {
  lcd.setCursor(0, 2);                
  lcd.print("Lege gemaess");                                                                                      // Text auf dem LCD anzeigen
  lcd.setCursor(0, 3);  
  lcd.print("Anzeige auf");
}

// Funktion zur Anzeige des Gewinners
void displayWinner(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int currentPlayer) {
  lcd.setCursor(0, 2);
  lcd.print("Gewonnen: ");                                                                                        // Text auf dem LCD anzeigen
  lcd.setCursor(0, 3);
  if (gameSettings.mode == 1) {
    lcd.print(currentPlayer == 1 ? "Spieler" : "Computer");
  } else {
    lcd.print(currentPlayer == 1 ? "Spieler 1" : "Spieler 2");
  }
}

// Funktion zur Anzeige eines Unentschieden
void displayDraw(LiquidCrystal_I2C &lcd) {
  lcd.setCursor(0, 2);
  lcd.print("Unentschieden!");                                                                                    // Text auf dem LCD anzeigen
  lcd.setCursor(0, 3);
  lcd.print("         ");
}

#endif