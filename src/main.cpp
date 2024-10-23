/*
  Projekt: Tapatan und Tic-Tac-Toe
  Mitwirkende: Eliane Hess, Aymeric Nouvier, Simeon Yasmin, Liam Mahieu, Jan Fawer
  Version: 1.0
*/

// Einbinden der Bibliotheken
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Einbinden der Header-Dateien
#include "struct.h"
#include "display.h"
#include "input.h"
#include "tictactoe.h"


LiquidCrystal_I2C lcd(0x27, 20, 4);                                                                     // Setze die Adresse des LCD-Displays

// Pins für die Potentiometer
const int numPotentiometers = 9;
const int potPins[numPotentiometers] = {A0, A1, A2, A3, A4, A5, A6, A7, A8};

// Spieleinstellungen
GameSettings gameSettings = {                                                           
  .game = 1,                                                                                            // Spiel
  .mode = 1,                                                                                            // Spielmodus
  .difficulty = 0,                                                                                      // Schwierigkeitsgrad
};

// Konstanten für die Spiele
const int TicTacToe = 1;                                                                                // Konstante für das Spiel Tic Tac Toe
const int Tapatan = 2;                                                                                  // Konstante für das Spiel Tapatan

int Board [3][3];                                                                                       // Array für das Spielfeld (0 = Leeres Feld, 1 = X, 2 = O)
int BoardMemory [3][3];                                                                                 // Array für das den Spielfeldspeicher (0 = Leeres Feld, 1 = X, 2 = O)
int ResetBoard[3][3] = {{0,0,0},{0,0,0},{0,0,0}};                                                       // Array für das Zurücksetzen des Spielfelds
int currentPlayer;                                                                                      // Variable für den Spieler der am Zug ist 1 = Spieler 1, 2 = Spieler 2 / Computer

// Setup Funktion
void setup() {
  lcd.begin(20, 4);                                                                                     // LCD initialisieren
  lcd.backlight();                                                                                      // Hintergrundbeleuchtung einschalten
  Serial.begin(9600);                                                                                   // Serielle Kommunikation starten
  displayStart(lcd);                                                                                    // Spielintialisierung
  randomSeed(analogRead(9));
  delay(1000);
}

// Loop Funktion
void loop() {
  readSensors(Board, numPotentiometers, potPins);                                                         // Sensorwerte auslesen
  copyArray(Board, BoardMemory);                                                                          // Aktuelles Spielfeld in den Speicher kopieren
  bool firstMove = true;                                                                                   // Variable für den ersten Zug
  currentPlayer = random(1, 3);                                                                            // Zufällige Auswahl des Startspielers

  if (isBoardEqual(Board, ResetBoard)) {                                                                  // Überprüfen, ob das Spielfeld zurückgesetzt wurde
    switch (gameSettings.game) {                                                                          // Auswahl des Spiels
    case TicTacToe:
      playTicTacToe(lcd, gameSettings, Board, BoardMemory, currentPlayer, numPotentiometers, potPins, firstMove);    // Spiel Tic Tac Toe starten
      delay(7000);
      break;
    case Tapatan:
      // playTapatan();                                                                                   // Spiel Tapatan starten
      break;
    default:
      Serial.println("Spiel nicht verfügbar");                                                            // Fehlermeldung, wenn das Spiel nicht verfügbar ist
      break;
    }
  } else {
    displayReset(lcd);                                                                                    // Zurücksetzen des Spielfelds anzeigen
    waitForReset(Board, numPotentiometers, potPins);                                                      // Warten auf das Zurücksetzen des Spielfelds
  }
}