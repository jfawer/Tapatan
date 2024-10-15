// Funktionen für die Spiellogik

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "input.h"
#include "display.h"

const int PlayerVsComputer = 1;                                         // Konstante für den Spielmodus Spieler gegen Computer
const int PlayerVsPlayer = 2;                                           // Konstante für den Spielmodus Spieler gegen Spieler
const int Player1 = 1;                                                  // Konstante für Spieler 1
const int Player2 = 2;                                                  // Konstante für Spieler 2

// Funktion zum wechseln des Spielers
void switchPlayer(int &turn) {
  turn = (turn == 1) ? 2 : 1;
}

// Funktion zur Überprüfung, ob jemand gewonnen hat
bool checkWin(int Board[3][3]) {
  // Überprüfen, ob eine Reihe gewonnen hat
  for (int i = 0; i < 3; i++) {
    if (Board[i][0] == Board[i][1] && Board[i][1] == Board[i][2] && Board[i][0] != 0) {
      return true;
    }
  }

  // Überprüfen, ob eine Spalte gewonnen hat
  for (int i = 0; i < 3; i++) {
    if (Board[0][i] == Board[1][i] && Board[1][i] == Board[2][i] && Board[0][i] != 0) {
      return true;
    }
  }

  // Überprüfen, ob eine Diagonale gewonnen hat
  if (Board[0][0] == Board[1][1] && Board[1][1] == Board[2][2] && Board[0][0] != 0) {
    return true;
  }

  if (Board[0][2] == Board[1][1] && Board[1][1] == Board[2][0] && Board[0][2] != 0) {
    return true;
  }

  return false;
}

// Funktion zur Überprüfung, ob es ein Unentschieden gibt
bool checkDraw(int Board[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (Board[i][j] == 0) {
        return false;
      }
    }
  }
  return true;
}

// Funktion für die Handhabung eines illegalen Zuges
void handleIllegalMove(LiquidCrystal_I2C lcd, int Board[3][3], int BoardSpeicher[3][3], int game, int mode, int turn ,const int numPotentiometers, const int potPins[]) {
  displayGameBoard(lcd, BoardSpeicher, game, mode, turn);
  displayIllegalMove(lcd);

  while (!isBoardEqual(Board, BoardSpeicher)) {                 // Warten, bis das Spielfeld zurückgesetzt wurde
    readSensors(Board, numPotentiometers, potPins);
    delay(100);
  }
}

// Funktion für das Spiel Tic Tac Toe Spieler gegen Spieler
void TicTacToePlayerVsPlayer(LiquidCrystal_I2C &lcd, int Board[3][3], int BoardSpeicher[3][3], int game, int mode, int &turn, int row, int col, const int numPotentiometers, const int potPins[]) {
  while (true) {                                                                                            // Endlosschleife für das Spiel
    readSensors(Board, numPotentiometers, potPins);                                                         // Auslesen der Sensorwerte
    displayGameBoard(lcd, Board, game, mode, turn);                                                         // Spielfeld anzeigen

    if ((turn == Player1) || (turn == Player2)) {                                                           // Überprüfen, ob ein Spieler am Zug ist          
      if (hasChanged(Board, BoardSpeicher)) {                                                               // Überprüfen, ob sich das Spielfeld geändert hat
        getChangedField(Board, BoardSpeicher, row, col);                                                    // Erkennen, welche Feld geändert wurde
        if (!isValidMove(Board, BoardSpeicher, row, col, turn)) {                                           // Überprüfen, ob der Zug ungültig ist
          handleIllegalMove(lcd, Board, BoardSpeicher, game, mode, turn, numPotentiometers, potPins);       // Unerlaubten Zug behandeln
        } else {
          copyArray(Board, BoardSpeicher);                                                                  // Aktuelles Spielfeld in den Speicher kopieren

          if (checkWin(Board)) {                                                                            // Überprüfen, ob jemand gewonnen hat   
            displayWinner(lcd, turn);                                                                       // Gewinner anzeigen     
            break;
          }

          if (checkDraw(Board)) {                                                                           // Überprüfen, ob es ein Unentschieden gibt
            displayDraw(lcd);                                                                               // Unentschieden anzeigen 
            break;
          }
          switchPlayer(turn);                                                                               // Spieler wechseln
        }
      }                                           
    }
    delay(300);
  }
}

// Funktion für das Spiel Tic Tac Toe
void playTicTacToe(LiquidCrystal_I2C &lcd, int Board[3][3], int BoardSpeicher[3][3], int game, int mode, int &turn, int difficulty, int row, int col,const int numPotentiometers,const int potPins[]) {
  // Spiellogik für Tic Tac Toe
  if (mode == PlayerVsComputer) {                                         
    // Spieler gegen Computer
  } else if (mode == PlayerVsPlayer) {                                                     
    TicTacToePlayerVsPlayer(lcd, Board, BoardSpeicher, game, mode, turn, row, col, numPotentiometers, potPins); // Spieler gegen Spieler
  }
}

#endif
