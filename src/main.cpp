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
#include "led.h"
#include "tictactoe.h"
#include "tapatan.h"

// Globale Variablen
volatile int rotarySwitchValue = 0;                                                                       // Aktueller Wert des Encoders

// Variablen für das LCD-Display
LiquidCrystal_I2C lcd(0x27, 20, 4);                                                                       // Setze die Adresse des LCD-Displays
byte umlautU[8] = { B01010, B00000, B10001, B10001, B10001, B10011, B01101, B00000 };                     // Umlaut ü
byte umlautO[8] = { B01010, B00000, B01110, B10001, B10001, B10001, B01110, B00000 };                     // Umlaut ö
byte umlautA[8] = { B01010, B00000, B01110, B00001, B01111, B10001, B01111, B00000 };                     // Umlaut ä

// Pins für die Potentiometer / Eingabesensoren
const int potPins[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8};                                               

// Pins für die Spielauswahl
const int rotarySwitchPin1 = 3;                                                                           // Pin für den Rotary-Encoder (CLK)
const int rotarySwitchPin2 = 2;                                                                           // Pin für den Rotary-Encoder (DT)
const int gameButtonPin = 4;                                                                              // Pin für den Bestätigungsknopf (SW)
const int redLED = 13;                                                                                    // Pin für die rote LED im LED Streifen
const int greenLED = 22;                                                                                  // Pin für die grüne LED im LED Streifen
const int blueLED = 12;                                                                                   // Pin für die blaue LED im LED Streifen
const int emptyAnalogPin = A10;                                                                           // Pin für den Seed der Zufallsfunktion

// Pins für die LEDs im LED Streifen
int ledPins[] = {redLED, greenLED, blueLED};                                                              // Pins für die LEDs im LED Streifen

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
bool setupvariable;                                                                                       // Variable für das Aktivieren des Encoders

// Interrupt Service Routine (ISR)
void handleEncoder() {
  if (setupvariable) {                                                                                    // Überprüfen, ob die Setupvariable gesetzt ist
    getRotaryValue(rotarySwitchPin1, rotarySwitchPin2, rotarySwitchValue);                                // Encoderwert auslesen
  }
}

// Setup Funktion
void setup() {
  pinMode(rotarySwitchPin1, INPUT_PULLUP);                                                                // Pin für den Rotary-Encoder (CLK) als Eingang
  pinMode(rotarySwitchPin2, INPUT_PULLUP);                                                                // Pin für den Rotary-Encoder (DT) als Eingang
  pinMode(gameButtonPin, INPUT_PULLUP);                                                                   // Pin für den Bestätigungsknopf (SW) als Eingang
  pinMode(greenLED, OUTPUT);                                                                              // Pin für die grüne LED im LED Streifen als Ausgang
  pinMode(redLED, OUTPUT);                                                                                // Pin für die rote LED im LED Streifen als Ausgang
  pinMode(blueLED, OUTPUT);                                                                               // Pin für die blaue LED im LED Streifen als Ausgang
  
  setupvariable = false;                                                                                  // Setupvariable auf false setzen

  attachInterrupt(digitalPinToInterrupt(rotarySwitchPin1), handleEncoder, CHANGE);                        // Interrupt für den Rotary-Encoder einrichten

  randomSeed(analogRead(emptyAnalogPin));                                                                 // Seed für die Zufallsfunktion
  Serial.begin(9600);                                                                                     // Serielle Kommunikation starten

  lcd.init();                                                                                             // LCD initialisieren
  lcd.createChar(0, umlautU);                                                                             // Umlaut Ü erstellen
  lcd.createChar(1, umlautO);                                                                             // Umlaut Ö erstellen
  lcd.createChar(2, umlautA);                                                                             // Umlaut Ä erstellen

  // Beleuchtung / Lichter
  lcd.backlight();                                                                                        // Hintergrundbeleuchtung einschalten
  steuereLED(Aus, ledPins);                                                                               // LED Streifen ausschalten
  
  displayStart(lcd);                                                                                      // Startbildschirm anzeigen                                                                               
  delay(2000);

  updateBoard(Board, potPins);                                                                            // Sensorwerte auslesen
  if (!isBoardEqual(Board, ResetBoard)) {                                                                 // Überprüfen, ob das Spielfeld nicht in der Ausgangsposition ist
    displayReset(lcd);                                                                                    // Anzeige zum Zurücksetzen des Spielfelds
    awaitBoardReset(Board, potPins);                                                                      // Warten auf das Zurücksetzen des Spielfelds
    resetGameSettings(gameSettings);                                                                      // Spieleinstellungen zurücksetzen
    copyBoard(ResetBoard, BoardMemory);                                                                   // Spielfeldspeicher zurücksetzen
  }
}

// Loop Funktion
void loop() {
  updateBoard(Board, potPins);                                                                            // Sensorwerte auslesen
  if (isBoardEqual(Board, ResetBoard) && gameSettings.game == 0) {                                        // Überprüfen, ob das Spielfeld in der Ausgangsposition ist und kein Spiel ausgewählt wurde
    // Spiel auswählen
    choseGameSettings(lcd, gameSettings, gameButtonPin, setupvariable);                                   // Spieleinstellungen auswählen
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