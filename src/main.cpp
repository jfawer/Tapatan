/*
  Projekt: Tapatan und Tic-Tac-Toe
  Autoren: Eliane Hess, Aymeric Nouvier, Simeon Yasmin, Liam Mahieu, Jan Fawer
  Version: 1.0
*/

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "Startinterface.h"
#include "Functions.h"
#include "Auslesen.h"


LiquidCrystal_I2C lcd(0x27, 20, 4);                                     // Setze die Adresse des LCD-Displays

const int numPotentiometers = 9;
const int potPins[numPotentiometers] = {A0, A1, A2, A3, A4, A5, A6, A7, A8};


int Board [3][3] = {{0,0,0},{0,0,0},{0,0,0}};                           //Array für das Spielfeld (0 = Leeres Feld, 1 = X, 2 = O)
int Boardspeicher [3][3] = {{0,0,0},{0,0,0},{0,0,0}};                   //Array für das Spielfeld (0 = Leeres Feld, 1 = X, 2 = O)

int game = 1;                                                           // Variable für das Spiel 1 = Tic Tac Toe, 2 = Tapatan
int mode = 2;                                                           // Variable für den Spielmodus 1 = Spieler gegen Computer, 2 = Spieler gegen Spieler
int difficulty = 0;                                                     // Variable für den Schwierigkeitsgrad des Computers 1 = Einfach, 2 = Schwer
int turn = 1;                                                           // Variable für den Spieler der am Zug ist 1 = Spieler 1, 2 = Spieler 2 / Computer

const int TicTacToe = 1;                                                // Konstante für das Spiel Tic Tac Toe
const int Tapatan = 2;                                                  // Konstante für das Spiel Tapatan

void setup() {
  lcd.begin(20, 4);                                                     // LCD initialisieren
  lcd.backlight();                                                      // Hintergrundbeleuchtung einschalten
  Serial.begin(9600);                                                   // Serielle Kommunikation starten
  setupGame(lcd);                                                       // Spielintialisierung
  delay(3000);
}

void loop() {

  // selectGame(game, mode, turn, difficulty);                          // Spielauswahl und Eingabe der Spielvariablen
  readSensors(Board, numPotentiometers, potPins);                       // Auslesen der Sensorwerte
  drawGameBoard(lcd, Board, game, mode, turn);                          // Zeige das Spielfeld auf dem LCD an

  if (hasChanged(Board, Boardspeicher)) {
    // Spieler wechseln
    turn = (turn == 1) ? 2 : 1;
  } else {
    // Nichts tun
  }
  copyArray(Board, Boardspeicher);                                      // Übertragen der Arraywerte eines Arrays in ein anderes Array
  delay(500);



  /*
  switch (game) {                                                             
  case TicTacToe:
    drawGameBoard(lcd, Board, game, mode, turn);                        // Zeige das Spielfeld auf dem LCD an
    delay(3000);
    playTicTacToe(lcd, Board, game, mode, turn, difficulty);            // Spiel Tic Tac Toe starten
    break;
  case Tapatan:
    // playTapatan();                                                   // Spiel Tapatan starten
    break;
  default:  
    break;
  }

  delay(5000);                                                          // Wartezeit nach Spielende
  resetGame(Board, game, mode, difficulty, turn);                       // Spiel zurücksetzen
  */
}
