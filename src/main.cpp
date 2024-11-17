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
#include "tapatan.h"

// Variablen für das LCD-Display
LiquidCrystal_I2C lcd(0x27, 20, 4);                                                                       // Setze die Adresse des LCD-Displays
byte umlautU[8] = { B01010, B00000, B10001, B10001, B10001, B10011, B01101, B00000 };                     // Umlaut ü
byte umlautO[8] = { B01010, B00000, B01110, B10001, B10001, B10001, B01110, B00000 };                     // Umlaut ö
byte umlautA[8] = { B01010, B00000, B01110, B00001, B01111, B10001, B01111, B00000 };                     // Umlaut ä

// Pins für die Potentiometer / Eingabesensoren
const int potPins[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8};                                               

// Pins für die Spielauswahl
const int gamePotPin = A9;
const int rotarySwitchPin1 = 3;         //23                                                                // Pin für den Potentiometer zur Auswahl des Spielmodus
const int rotarySwitchPin2 = 4;         //25                                                                // Pin für den Potentiometer zur Auswahl des Spiels
const int gameButtonPin = 2;          //24                                                                // Pin für den Knopf zur Bestätigung des Spiels
const int emptyAnalogPin = A10;                                                                           // Pin für den Seed der Zufallsfunktion

// Spieleinstellungen
GameSettings gameSettings = {                                                           
  .game = 0,                                                                                              // Spiel / 1: Tic Tac Toe, 2: Tapatan
  .mode = 0,                                                                                              // Spielmodus / 1: Spieler gegen Computer, 2: Spieler gegen Spieler
  .difficulty = 0,                                                                                        // Schwierigkeitsgrad / 1: Einfach, 2: Mittel, 3: Schwer
};

// Konstanten für die Spiele
const int TicTacToe = 1;                                                                                  // Konstante für das Spiel Tic Tac Toe
const int Tapatan = 2;                                                                                    // Konstante für das Spiel Tapatan

int Board [3][3];                                                                                         // Array für das Spielfeld (0 = Leeres Feld, 1 = X, 2 = O)
int BoardMemory [3][3];                                                                                   // Array für das den Spielfeldspeicher (0 = Leeres Feld, 1 = X, 2 = O)
int ResetBoard[3][3] = {{0,0,0},{0,0,0},{0,0,0}};                                                         // Array für das Zurücksetzen des Spielfelds
int currentPlayer;                                                                                        // Variable für den Spieler der am Zug ist 1 = Spieler 1, 2 = Spieler 2 / Computer
int lastRotarySwitchState;                                                                                // Variable für den letzten Zustand des Rotary-Switch
int rotarySwitchValue;                                                                                    // Variable für den Wert des Rotary-Switch

// Setup Funktion
void setup() {
  setupRotarySwitch();                                                                                    // Rotary-Switch initialisieren
  randomSeed(analogRead(emptyAnalogPin));                                                                 // Seed für die Zufallsfunktion
  Serial.begin(9600);                                                                                     // Serielle Kommunikation starten

  lcd.init();                                                                                             // LCD initialisieren
  lcd.createChar(0, umlautU);                                                                             // Umlaut Ü erstellen
  lcd.createChar(1, umlautO);                                                                             // Umlaut Ö erstellen
  lcd.createChar(2, umlautA);                                                                             // Umlaut Ä erstellen
  lcd.backlight();                                                                                        // Hintergrundbeleuchtung einschalten
  
  displayStart(lcd);                                                                                      // Startbildschirm anzeigen                                                                               
  delay(2000);
}

// Loop Funktion
void loop() {
  
  readRotarySwitch();                                                                                    // Rotary-Switch auslesen
  // Überprüfen, ob das Spielfeld in der Ausgangsposition ist
  updateBoard(Board, potPins);                                                                            // Sensorwerte auslesen
  if (isBoardEqual(Board, ResetBoard) && gameSettings.game == 0) {                                        // Überprüfen, ob das Spielfeld in der Ausgangsposition ist und kein Spiel ausgewählt wurde
    // Spiel auswählen
    choseGameSettings(lcd, gameSettings, gamePotPin, gameButtonPin);                                      // Spielmodus, Schwierigkeitsgrad und Spiel auswählen
    currentPlayer = random(1, 3);                                                                         // Zufällige Auswahl des Startspielers
    delay(500);

    // Spiel starten
    switch (gameSettings.game) {                                                                          // Auswahl des Spiels
      case TicTacToe:
        playTicTacToe(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);                     // Spiel Tic Tac Toe starten
        delay(5000);
        break;
      case Tapatan:
        playTapatan(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);                       // Spiel Tapatan starten
        delay(5000);
        break;
    }
  } else {
    displayReset(lcd);                                                                                    // Anzeige zum Zurücksetzen des Spielfelds
    awaitBoardReset(Board, potPins);                                                                      // Warten auf das Zurücksetzen des Spielfelds
    resetGameSettings(gameSettings);                                                                      // Spieleinstellungen zurücksetzen
    copyBoard(ResetBoard, BoardMemory);                                                                   // Spielfeldspeicher zurücksetzen
  }
}