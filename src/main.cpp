/*
  Projekt: Tapatan und Tic-Tac-Toe
  Autoren: Eliane Hess, Aymeric Nouvier, Simeon Yasmin, Liam Mahieu, Jan Fawer
  Version: 1.0
*/

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Functions.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);                                     // Setze die Adresse des LCD-Displays

int gameBoard [3][3] = {{0,0,0},{0,0,0},{0,0,0}};                       //Array für das Spielfeld (0 = Leeres Feld, 1 = X, 2 = O)

int gameType = 1;                                                       // Variable für den Spieltyp 1 = Tic Tac Toe, 2 = Tapatan
int gameMode = 0;                                                       // Variable für den Spielmodus 1 = Spieler gegen Computer, 2 = Spieler gegen Spieler
int turn = 0;                                                           // Variable für den Spieler der am Zug ist 1 = Spieler 1, 2 = Spieler 2 / Computer
int difficulty = 0;                                                     // Variable für den Schwierigkeitsgrad des Computers 1 = Einfach, 2 = Schwer

void setup() {
  lcd.begin(20, 4);                                                     // LCD initialisieren
  lcd.backlight();                                                      // Hintergrundbeleuchtung einschalten
  lcd.clear();                                                          // LCD löschen
  Serial.begin(9600);                                                   // Serielle Kommunikation starten
}

void loop() {
  if (gameType == 1) {                                                  // Spiel Tic Tac Toe starten                    
    // TicTacToe();
    inputGameBoard(gameBoard);                                          // Eingabe des Spielfelds
    displayBoard(lcd, gameBoard, gameType, gameMode, turn);             // Zeige das Spielfeld auf dem LCD an

  } else if (gameType == 2) {               
    // Tapatan();                                                       // Spiel Tapatan starten
  } else {                                   
    inputGameVariables(gameType, gameMode, turn, difficulty);           // Eingabe der Spielvariablen
    displayBoard(lcd, gameBoard, gameType, gameMode, turn);             // Zeige das Spielfeld auf dem LCD an
  }
}
