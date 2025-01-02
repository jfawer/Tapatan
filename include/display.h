// Funktionen zur Anzeige von Texten auf dem LCD-Display und zur Eingabe von Spielvariablen

#ifndef DISPLAY_H
#define DISPLAY_H

// Einbinden der Bibliotheken
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Einbinden der Header-Dateien
#include "input.h"
#include "struct.h"
#include "led.h"

extern volatile int rotarySwitchValue;
extern LED led;

// ====================================================================================================
// Funktionen für die Anzeige des Tic Tac Toe Spielfelds
// ====================================================================================================

// Funktion für die Anzeige des Boards
void displayBoard(LiquidCrystal_I2C &lcd, int Board[3][3]) {
  for (int row = 0; row < 3; row++) {                                                                             // Zeichne das Spielfeld
    for (int col = 0; col < 3; col++) {
      lcd.setCursor(col + 15, row + 1);
      char symbol = (Board[row][col] == 0) ? '_' : (Board[row][col] == 1) ? 'X' : 'O';                            // Entscheide, welches Symbol angezeigt werden soll
      lcd.print(symbol);
    }
  }
}

// Funktion zur Anzeige des Spielers, der am Zug ist
void displayPlayer(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int currentPlayer) {
  lcd.setCursor(0, 2);
  lcd.print("Am Zug:      ");                                                                                     // Zeige den Spieler, der am Zug ist
  lcd.setCursor(0, 3);                                                                                    
  if (gameSettings.mode == 1) {                                                                                   // Überprüfen, ob der Spieler gegen den Computer spielt
    lcd.print(currentPlayer == 1 ? "Spieler   (X)" : "Computer  (O)");
  } else {
    lcd.print(currentPlayer == 1 ? "Spieler 1 (X)" : "Spieler 2 (O)");
  }
}

// Funktion zur Anzeige des Spielfelds
void displayGameScreen(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board[3][3], int currentPlayer) {
  lcd.clear();                                                                                                    // LCD löschen
  lcd.setCursor(0, 0);
  lcd.print(gameSettings.game == 1 ? "Tic Tac Toe:" : "Tapatan:");                                                // Zeige den Namen des Spiels
  displayPlayer(lcd, gameSettings, currentPlayer);                                                                // Zeige den Spieler, der am Zug ist
  displayBoard(lcd, Board);                                                                                       // Spielfeld anzeigen
}


// ====================================================================================================
// Funktionen für die Anzeige der Spielinformationen
// ====================================================================================================

// Funktion zur Anzeige zum Zurücklegen einer unerlaubten Bewegung
void displayIllegalMove(LiquidCrystal_I2C &lcd) {
  lcd.setCursor(0, 2);                
  lcd.print("Spielfigur");                                                                                     
  lcd.setCursor(0, 3);  
  lcd.print("zur");
  lcd.write(byte(0));                                                                                             // Umlaut Ü anzeigen
  lcd.print("cklegen  ");                                                                                           
}

// Funktion zur Anzeige des Gewinners
void displayWinner(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int currentPlayer) {
  lcd.setCursor(0, 2);
  lcd.print("Gewonnen: ");                                                                                       
  lcd.setCursor(0, 3);
  if (gameSettings.mode == 1) {                                                                                   // Überprüfen, ob der Spieler gegen den Computer spielt
    led.setColor(currentPlayer == 1 ? "Blau" : "Rot");                                                            // LED Streifen auf die Farbe des Gewinners setzen
    lcd.print(currentPlayer == 1 ? "Spieler" : "Computer");
  } else {
    led.setColor(currentPlayer == 1 ? "Blau" : "Rot");                                                            // LED Streifen auf die Farbe des Gewinners setzen
    lcd.print(currentPlayer == 1 ? "Spieler 1" : "Spieler 2");
  }
}

// Funktion zur Anzeige eines Unentschieden
void displayDraw(LiquidCrystal_I2C &lcd) {
  led.setColor("lila");                                                                                           // LED Streifen auf lila setzen
  lcd.setCursor(0, 2);
  lcd.print("Unentschieden!");                                                                                   // Text auf dem LCD anzeigen 
  lcd.setCursor(0, 3);
  lcd.print("              ");
}


// ====================================================================================================
// Funktionen für die Benutzeroberfläche
// ====================================================================================================

// Funktion für die Anzeige des Startbildschirms
void displayStart(LiquidCrystal_I2C lcd) {
  lcd.clear();                                                                                                    // LCD löschen
  lcd.setCursor(6, 1);
  lcd.print("Herzlich");                                                                                          // Text auf dem LCD anzeigen
  lcd.setCursor(5, 2);
  lcd.print("Willkommen");                                                                                        // Text auf dem LCD anzeigen
}

// Funktion für die Anzeige zum Zurücksetzen des Spielfelds
void displayReset(LiquidCrystal_I2C lcd) {
  lcd.clear();                                                                                                    // LCD löschen
  lcd.setCursor(4, 1);
  lcd.print("Zur");                                                                                               
  lcd.write(byte(0));                                                                                             // Umlaut Ü anzeigen
  lcd.print("cksetzen");                                                                                               
  lcd.setCursor(3, 2);
  lcd.print("des Spielfelds");
}


// ====================================================================================================
// Funktionen für die Spielauswahl
// ====================================================================================================

// Funktion zur Anzeige des Auswahlsymbols
void displaySelectionSymbol(LiquidCrystal_I2C &lcd, int bereich) {                                                                                   
  for (int i = 1; i <= 3; i++) {                                                                                  // Auswahl anzeigen
    if (i == bereich) {                                                                                           // Überprüfen, ob der Bereich ausgewählt ist
      lcd.setCursor(0, i);
      lcd.print("*");                                                                                             
    } else {
      lcd.setCursor(0, i);
      lcd.print(" ");                                                                                             
    }                                                                                              
  }
}

// Funktion zur Anzeige der Spielauswahl
void displayGameSelection(LiquidCrystal_I2C &lcd) {
  lcd.clear();                                                                                                    // LCD löschen
  lcd.setCursor(0, 0);
  lcd.print("Spiel ausw");
  lcd.write(byte(2));                                                                                             // Umlaut Ä anzeigen
  lcd.print("hlen:");                                                                                
  lcd.setCursor(2, 1);
  lcd.print("Tic Tac Toe");                                                                                       // Text auf dem LCD anzeigen
  lcd.setCursor(2, 2);
  lcd.print("Tapatan");                                                                                           // Text auf dem LCD anzeigen
}

// Funktion zur Auswahl des Spiels
void choseGame(LiquidCrystal_I2C &lcd, GameSettings &gameSettings, int gameButtonPin) {
  static bool gameSelectionDisplayed = false;                                                                     // Statische Variable für die einmalige Anzeige der Spielauswahl
  if (!gameSelectionDisplayed) {                                                                                  // Überprüfen, ob die Spielauswahl angezeigt wurde
    displayGameSelection(lcd);                    
    gameSelectionDisplayed = true; 
  }

  int bereich = getRotarySwitchRangeValue(2, rotarySwitchValue);                                                  // Bereich berechnen
  if (!fallendeFlanke(gameButtonPin)) {                                                                           // Überprüfen, ob der Knopf gedrückt wurde
    displaySelectionSymbol(lcd, bereich);
  } else {
    gameSettings.game = bereich;                                                                                  // Spiel auswählen
    gameSelectionDisplayed = false;
  }
}

// Funktion zur Anzeige der Modusauswahl
void displayModeSelection(LiquidCrystal_I2C &lcd) {
  lcd.clear();                                                                                                   // LCD löschen
  lcd.setCursor(0, 0);
  lcd.print("Modus ausw");
  lcd.write(byte(2));                                                                                            // Umlaut Ä anzeigen
  lcd.print("hlen:"); 
  lcd.setCursor(2, 1);
  lcd.print("P vs. C");                                                                                          // Text auf dem LCD anzeigen
  lcd.setCursor(2, 2);
  lcd.print("P vs. P");                                                                                          // Text auf dem LCD anzeigen
}

// Funktion zur Auswahl des Modus
void choseMode(LiquidCrystal_I2C &lcd, GameSettings &gameSettings, int gameButtonPin) {
  static bool modeSelectionDisplayed = false;                                                                     // Statistische Variable für die einmalige Anzeige  
  if (!modeSelectionDisplayed) {                                                                                  // Überprüfen, ob die Modusauswahl angezeigt wurde
    displayModeSelection(lcd);
    modeSelectionDisplayed = true;
  }

  int bereich = getRotarySwitchRangeValue(2, rotarySwitchValue);                                                  // Bereich berechnen
  if (!fallendeFlanke(gameButtonPin)) {                                                                           // Überprüfen, ob der Knopf gedrückt wurde
    displaySelectionSymbol(lcd, bereich);
  } else {
    gameSettings.mode = bereich;                                                                                  // Modus auswählen
    modeSelectionDisplayed = false;
  }
}

// Funktion zur Anzeige der Schwierigkeitsauswahl
void displayDifficultySelection(LiquidCrystal_I2C &lcd) {
  lcd.clear();                                                                                                   // LCD löschen
  lcd.setCursor(0, 0);
  lcd.print("Schwierigkeit:");                                                                                   // Text auf dem LCD anzeigen
  lcd.setCursor(2, 1);
  lcd.print("Einfach");                                                                                          // Text auf dem LCD anzeigen
  lcd.setCursor(2, 2);
  lcd.print("Mittel");                                                                                           // Text auf dem LCD anzeigen
  lcd.setCursor(2, 3);
  lcd.print("Schwer");                                                                                           // Text auf dem LCD anzeigen
}

// Funktion zur Auswahl des Schwierigkeitsgrades
void choseDifficulty(LiquidCrystal_I2C &lcd, GameSettings &gameSettings, int gameButtonPin) {
  static bool difficultySelectionDisplayed = false;                                                               // Statische Variable für die einmalige Anzeige
  if (!difficultySelectionDisplayed) {                                                                            // Überprüfen, ob die Schwierigkeitsauswahl angezeigt wurde
    displayDifficultySelection(lcd);
    difficultySelectionDisplayed = true;
  }

  int bereich = getRotarySwitchRangeValue(3, rotarySwitchValue);                                                  // Bereich berechnen
  if (!fallendeFlanke(gameButtonPin)) {                                                                           // Überprüfen, ob der Knopf gedrückt wurde
    displaySelectionSymbol(lcd, bereich);
  } else {
    gameSettings.difficulty = bereich;                                                                            // Schwierigkeitsgrad auswählen
    difficultySelectionDisplayed = false;
  }
}

// Funktion zur Auswahl der Spielvariablen
void choseGameSettings(LiquidCrystal_I2C &lcd, GameSettings &gameSettings, int gameButtonPin, bool &setupvariable) {
  setupvariable = true;                                                                                           // Encoder aktivieren
  lcd.clear();                                                                                                    // LCD löschen
  const int PlayerVsComputer = 1;
  while (gameSettings.game == 0) {                                                                                
    choseGame(lcd, gameSettings, gameButtonPin);                                                                  // Spiel auswählen      
  delay(500);
  }
  while (gameSettings.mode == 0) {                                                                                
    choseMode(lcd, gameSettings, gameButtonPin);                                                                  // Modus auswählen
  }
  delay(500);
  while (gameSettings.mode == PlayerVsComputer && gameSettings.difficulty == 0) {
    choseDifficulty(lcd, gameSettings, gameButtonPin);                                                            // Schwierigkeitsgrad auswählen
  }
  setupvariable = false;                                                                                          // Encoder deaktivieren
}

#endif