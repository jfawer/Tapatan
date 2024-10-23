#ifndef TICTACTOE_H
#define TICTACTOE_H

// Einbinden der Bibliotheken
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Einbinden der Header-Dateien
#include "struct.h"
#include "game_logic.h"
#include "display.h"
#include "input.h"

const int PlayerVsComputer = 1;                                         // Konstante für den Spielmodus Spieler gegen Computer
const int PlayerVsPlayer = 2;                                           // Konstante für den Spielmodus Spieler gegen Spieler
const int Player1 = 1;                                                  // Konstante für Spieler 1
const int Player2 = 2;                                                  // Konstante für Spieler 2

// Funktion für das Spiel Tic Tac Toe Spieler gegen Spieler
void TicTacToePlayerVsPlayer(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board [3][3], int BoardMemory[3][3], int currentPlayer, const int numPotentiometers, const int potPins[]) {
  while (true) {                                                                                                  // Endlosschleife für das Spiel
    readSensors(Board, numPotentiometers, potPins);                                                               // Sensorwerte auslesen
    displayGameBoard(lcd, gameSettings, Board, currentPlayer);                                                    // Spielfeld anzeigen
    bool turnOver = false;

    if ((currentPlayer == Player1) || (currentPlayer == Player2)) {                                               // Überprüfen, ob ein Spieler am Zug ist          
      if (hasChanged(Board, BoardMemory)) {                                                                       // Überprüfen, ob sich das Spielfeld geändert hat
        getChangedField(Board, BoardMemory);                                                                      // Erkennen, welche Feld geändert wurde
        if (isValidMove(Board, BoardMemory, currentPlayer)) {                                                     // Überprüfen, ob der Zug ungültig ist
          turnOver = true;                                                                                        // Zug beenden
        } else {
          displayGameBoard(lcd, gameSettings, BoardMemory, currentPlayer);                                        // Spielfeld anzeigen
          displayIllegalMove(lcd);
          waitUntilMoveIsUndone(Board, BoardMemory, numPotentiometers, potPins);                                  // Warten auf das Zurücksetzen des Spielfelds
        }
      }                                           
    }

    if (turnOver) {                                                                                               // Überprüfen, ob der Zug beendet wurde
      copyBoard(Board, BoardMemory);                                                                              // Aktuelles Spielfeld in den Speicher kopieren

      if (evaluate(Board) == 10) {                                                                                // Überprüfen, ob Spieler 1 gewonnen hat
        displayWinner(lcd, gameSettings, Player1);                                                                // Gewinner anzeigen
        break;
      } else if (evaluate(Board) == -10) {                                                                        // Überprüfen, ob Spieler 2 / Computer gewonnen hat
        displayWinner(lcd, gameSettings, Player2);                                                                // Gewinner anzeigen
        break;
      } else if (!checkFieldsLeft(Board)) {                                                                       // Überprüfen, ob es ein Unentschieden gibt
        displayDraw(lcd);                                                                                         // Unentschieden anzeigen
        break;
      }
      switchPlayer(currentPlayer);                                                                                // Spieler wechseln
    }
    delay(300);
  }
}

// Funktion für das Spiel Tic Tac Toe Spieler gegen Computer
void TicTacToePlayerVsComputer(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board [3][3], int BoardMemory[3][3], int currentPlayer, const int numPotentiometers, const int potPins[], bool &firstMove) {
  // Spiellogik für Tic Tac Toe
  while (true){
    readSensors(Board, numPotentiometers, potPins);                                                               // Sensorwerte auslesen
    displayGameBoard(lcd, gameSettings, Board, currentPlayer);                                                    // Spielfeld anzeigen
    bool turnOver = false;

    if (currentPlayer == Player1) {                                                                               // Überprüfen, ob Spieler 1 am Zug ist       
      if (hasChanged(Board, BoardMemory)) {                                                                       // Überprüfen, ob sich das Spielfeld geändert hat
        getChangedField(Board, BoardMemory);                                                                      // Erkennen, welche Feld geändert wurde
        if (isValidMove(Board, BoardMemory, currentPlayer)) {                                                     // Überprüfen, ob der Zug ungültig ist
          turnOver = true;                                                                                        // Zug beenden
        } else {
          displayGameBoard(lcd, gameSettings, BoardMemory, currentPlayer);                                        // Spielfeld anzeigen
          displayIllegalMove(lcd);
          waitUntilMoveIsUndone(Board, BoardMemory, numPotentiometers, potPins);                                  // Warten auf das Zurücksetzen des Spielfelds
        }
      }                                           
    } else if (currentPlayer == Player2) {
      // Computerzug
      int BoardDisplay[3][3];
      copyBoard(Board, BoardDisplay);
      makeBestMove(BoardDisplay, 9);                                                                              // Besten Zug für den Computer bestimmen
      displayGameBoard(lcd, gameSettings, BoardDisplay, currentPlayer);                                           // Spielfeld anzeigen
      waitUntilMoveIsUndone(Board, BoardDisplay, numPotentiometers, potPins);                                     // Warten, bis der Computerzug gemacht wurde
      turnOver = true;                                                                                            // Zug beenden
    }

    if (turnOver) {   
      readSensors(Board, numPotentiometers, potPins);                                                             // Sensorwerte auslesen
      // Ausgabe des Spielfelds in der Konsole
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          Serial.print(Board[i][j]);
          Serial.print(" ");
        }
        Serial.println();
      }

      copyBoard(Board, BoardMemory);                                                                              // Aktuelles Spielfeld in den Speicher kopieren
      Serial.print("Evaluate: ");
      Serial.println(evaluate(Board));

      if (evaluate(Board) == 10) {                                                                                // Überprüfen, ob Spieler 1 gewonnen hat
        displayWinner(lcd, gameSettings, Player1);                                                                // Gewinner anzeigen
        break;
      } else if (evaluate(Board) == -10) {                                                                        // Überprüfen, ob Spieler 2 gewonnen hat
        displayWinner(lcd, gameSettings, Player2);                                                                // Gewinner anzeigen
        break;
      } else if (!checkFieldsLeft(Board)) {                                                                       // Überprüfen, ob es ein Unentschieden gibt
        displayDraw(lcd);                                                                                         // Unentschieden anzeigen
        break;
      }
      switchPlayer(currentPlayer);                                                                                // Spieler wechseln
    }
    delay(300);
  }
}

// Funktion für das Spiel Tic Tac Toe
void playTicTacToe(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board [3][3], int BoardMemory[3][3], int currentPlayer, const int numPotentiometers, const int potPins[], bool &firstMove) {
  // Spiellogik für Tic Tac Toe
  if (gameSettings.mode == PlayerVsComputer) {                                         
    TicTacToePlayerVsComputer(lcd, gameSettings, Board, BoardMemory, currentPlayer, numPotentiometers, potPins, firstMove);  // Spieler gegen Computer
  } else if (gameSettings.mode == PlayerVsPlayer) {                                                     
    TicTacToePlayerVsPlayer(lcd, gameSettings, Board, BoardMemory, currentPlayer, numPotentiometers, potPins);    // Spieler gegen Spieler
  }
}


#endif