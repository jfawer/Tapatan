// Funktionen zur Anzeige von Texten auf dem LCD-Display und zur Eingabe von Spielvariablen

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Funktion für die Anzeige des Startbildschirms
void displayStart(LiquidCrystal_I2C lcd) {
  lcd.clear();                                                                                            // LCD löschen
  lcd.setCursor(0, 0);
  lcd.print("Tapatan und");                                                                               // Text auf dem LCD anzeigen       
  lcd.setCursor(0, 1);
  lcd.print("Tic Tac Toe");
}

// Funktion zum Eingeben der Spielvariablen
void selectGame(int &game, int &mode, int &difficulty, int &turn) {
  String input;

  // Spieltyp eingeben
  Serial.println("Gib den Spieltyp ein (1 = Tic Tac Toe, 2 = Tapatan):");
  while (Serial.available() == 0) {}                                                                      // Warten auf Eingabe
  game = Serial.parseInt();                                                                               // Lesen der Eingabe
  Serial.print("Gewählter Spieltyp: ");
  Serial.println(game);

  // Spielmodus eingeben
  Serial.println("Gib den Spielmodus ein (1 = Spieler gegen Computer, 2 = Spieler gegen Spieler):");
  while (Serial.available() == 0) {}                                                                      // Warten auf Eingabe
  mode = Serial.parseInt();                                                                               // Lesen der Eingabe
  Serial.print("Gewählter Spielmodus: ");
  Serial.println(mode);

  // Schwierigkeitsgrad eingeben (nur bei Spieler gegen Computer)
  if (mode == 1) {
    Serial.println("Gib den Schwierigkeitsgrad ein (1 = Einfach, 2 = Schwer):");
    while (Serial.available() == 0) {}                                                                    // Warten auf Eingabe
    difficulty = Serial.parseInt();                                                                       // Lesen der Eingabe
    Serial.print("Gewählter Schwierigkeitsgrad: ");
    Serial.println(difficulty);
  } else {
    difficulty = 0;                                                                                       // Setzt die Schwierigkeit auf 0, wenn Spieler gegen Spieler gespielt wird
  }

  // Zufällige Auswahl des Startspielers
  randomSeed(analogRead(0));                                                                              // Initialisieren des Zufallsgenerators                    
  turn = random(1,3);                                                                                     // Zufällige Auswahl des Startspielers (1 oder 2)
}

// Funktion zum Zeichnen Spielfelds
void displayGameBoard(LiquidCrystal_I2C &lcd, int Board[3][3], int game, int mode, int turn) {
  lcd.clear();                                                                                            // LCD löschen

  lcd.setCursor(0, 0);
  lcd.print(game == 1 ? "Tic Tac Toe:" : "Tapatan:");                                                     // Zeige den Namen des Spiels

  lcd.setCursor(0, 2);
  lcd.print("Am Zug:");                                                                                   // Zeige den Spieler, der am Zug ist               
  lcd.setCursor(0, 3);                                                                                    
  if (mode == 1) {
    lcd.print(turn == 1 ? "Spieler" : "Computer");
  } else {
    lcd.print(turn == 1 ? "Spieler 1" : "Spieler 2");
  }
  
  for (int row = 0; row < 3; row++) {                                                                     // Zeichne das Spielfeld
    for (int col = 0; col < 3; col++) {
      lcd.setCursor(col + 15, row + 1);
      
      
      char symbol = (Board[row][col] == 0) ? '0' : (Board[row][col] == 1) ? 'X' : 'O';                    // Entscheide, welches Symbol angezeigt werden soll
      lcd.print(symbol);
    }
  }
}

// Funktion zur Anzeige zum Zurücklegen einer unerlaubten Bewegung
void displayIllegalMove(LiquidCrystal_I2C &lcd) {
  lcd.setCursor(0, 2);                
  lcd.print("Lege gemaess");                                                                              // Text auf dem LCD anzeigen
  lcd.setCursor(0, 3);  
  lcd.print("Anzeige auf");
}

// Funktion zur Anzeige des Gewinners
void displayWinner(LiquidCrystal_I2C &lcd, int turn) {
  lcd.setCursor(0, 2);
  lcd.print("Gewonnen: ");                                                                                // Text auf dem LCD anzeigen
  lcd.setCursor(0, 3);
  lcd.print(turn == 1 ? "Spieler 1" : "Spieler 2");
}

// Funktion zur Anzeige eines Unentschieden
void displayDraw(LiquidCrystal_I2C &lcd) {
  lcd.setCursor(0, 2);
  lcd.print("Unentschieden!");                                                                            // Text auf dem LCD anzeigen
  lcd.setCursor(0, 3);
  lcd.print("         ");
}

#endif