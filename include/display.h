// Funktionen zur Anzeige von Texten auf dem LCD-Display und zur Eingabe von Spielvariablen

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Funktion für die Spielintialisierung
void setupGame(LiquidCrystal_I2C lcd) {
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

#endif