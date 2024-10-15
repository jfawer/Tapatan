/*
  Projekt: Tapatan und Tic-Tac-Toe
  Autoren: Eliane Hess, Aymeric Nouvier, Simeon Yasmin, Liam Mahieu, Jan Fawer
  Version: 1.0
*/

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Einbinden der Header-Dateien
#include "display.h"
#include "game_logic.h"
#include "input.h"
#include "motor_control.h"
#include "magnet_control.h"


LiquidCrystal_I2C lcd(0x27, 20, 4);                                               // Setze die Adresse des LCD-Displays

// Pins für die Potentiometer
const int numPotentiometers = 9;
const int potPins[numPotentiometers] = {A0, A1, A2, A3, A4, A5, A6, A7, A8};
const int TicTacToe = 1;                                                          // Konstante für das Spiel Tic Tac Toe
const int Tapatan = 2;                                                            // Konstante für das Spiel Tapatan


int Board [3][3] = {{0,0,0},{0,0,0},{0,0,0}};                                     // Array für das Spielfeld (0 = Leeres Feld, 1 = X, 2 = O)
int BoardSpeicher [3][3] = {{0,0,0},{0,0,0},{0,0,0}};                             // Array für das den Spielfeldspeicher (0 = Leeres Feld, 1 = X, 2 = O)
int row = 0;                                                                      // Variable für die Zeile des Spielfelds
int col = 0;                                                                      // Variable für die Spalte des Spielfelds

int game = 1;                                                                     // Variable für das Spiel 1 = Tic Tac Toe, 2 = Tapatan
int mode = 2;                                                                     // Variable für den Spielmodus 1 = Spieler gegen Computer, 2 = Spieler gegen Spieler
int difficulty = 0;                                                               // Variable für den Schwierigkeitsgrad des Computers 1 = Einfach, 2 = Schwer
int turn = 1;                                                                     // Variable für den Spieler der am Zug ist 1 = Spieler 1, 2 = Spieler 2 / Computer

void setup() {
  lcd.begin(20, 4);                                                               // LCD initialisieren
  lcd.backlight();                                                                // Hintergrundbeleuchtung einschalten
  Serial.begin(9600);                                                             // Serielle Kommunikation starten
  displayStart(lcd);                                                              // Spielintialisierung
  delay(1000);
}

void loop() {

  switch (game) {                                                                                                   // Auswahl des Spiels
  case TicTacToe:
    playTicTacToe(lcd, Board, BoardSpeicher, game, mode, turn, difficulty, row, col, numPotentiometers, potPins);   // Spiel Tic Tac Toe starten
    game = 0;                                                                                                       // Spiel beenden
    break;
  case Tapatan:
    // playTapatan();                                                                                               // Spiel Tapatan starten
    break;
  default:
    Serial.println("Spiel nicht verfügbar");  
    break;
  }
}