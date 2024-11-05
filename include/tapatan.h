#ifndef TAPATAN_H
#define TAPATAN_H

// Einbinden der Bibliotheken
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Einbinden der Header-Dateien
#include "struct.h"
#include "input.h"
#include "game_logic.h"
#include "display.h"


// Funktion zum Überprüfen, ob das Spielfeld geändert wurde und der Zug gültig ist
bool playerPlaces(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board[3][3], int BoardMemory[3][3], int currentPlayer, const int potPins[]) {
  bool turnOver = false;
    
  if (hasBoardChanged(Board, BoardMemory)) {
    if (isValidMove(Board, BoardMemory, currentPlayer)) {
      displayBoard(lcd, Board);
      turnOver = true;
    } else {
      displayBoard(lcd, BoardMemory);
      displayIllegalMove(lcd);
      awaitBoardIsEqual(Board, BoardMemory, potPins);
      displayPlayer(lcd, gameSettings, currentPlayer);
    }
  }
  return turnOver;
}

// Funktion zum Zählen der Spielsteine auf dem Spielfeld eines Spielers
int countPlayerStones(int Board[3][3], int player) {
    int count = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (Board[i][j] == player) {
                count++;
            }
        }
    }
    return count;
}

// Funktion zum Überprüfen, die Verschiebung des Spielsteins gültig ist
bool isValidPlacement(int currentBoard[3][3], int savedBoard[3][3], int currentPlayer, BoardField originField) {
    BoardField changedField = getChangedField(currentBoard, savedBoard);
    int row = changedField.row;
    int col = changedField.col;

    if (row == -1 && col == -1) {
        return false; // Kein Feld wurde geändert
    }

    int oldValue = savedBoard[row][col];   
    int newValue = currentBoard[row][col];

    // Überprüfen, ob das geänderte Feld leer war, vom aktuellen Spieler geändert wurde,
    // nicht das Ursprungsfeld ist und innerhalb eines Radius von 1 Feld liegt
    if (oldValue == 0 && newValue == currentPlayer && 
        (row != originField.row || col != originField.col) &&
        abs(row - originField.row) <= 1 && abs(col - originField.col) <= 1) {
        return true;
    } else {
        return false;
    }
}


// Funktion für den Spielmodus Tapatan Spieler gegen Spieler
void TapatanPlayerVsPlayer(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board[3][3], int BoardMemory[3][3], int currentPlayer, const int potPins[]) {
    bool firstDisplay = true;
    bool turnOver = false;
    int turn = 0;

    while (true) {
        if (firstDisplay) {
            displayGameScreen(lcd, gameSettings, Board, currentPlayer);
            firstDisplay = false;
        }

        int otherPlayer = currentPlayer == Player1 ? Player2 : Player1;
        updateBoard(Board, potPins);

        if (turn < 6) {
            turnOver = playerPlaces(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);
        } else {
            if (hasBoardChanged(Board, BoardMemory)) {

                if (countPlayerStones(Board, otherPlayer) == 2) {
                    displayBoard(lcd, BoardMemory);
                    displayIllegalMove(lcd);
                    awaitBoardIsEqual(Board, BoardMemory, potPins);
                    displayPlayer(lcd, gameSettings, currentPlayer);

                } else if (countPlayerStones(Board, currentPlayer) == 2) {

                    displayBoard(lcd, Board);
                    BoardField originField = getChangedField(Board, BoardMemory);
                    copyBoard(Board, BoardMemory);

                    do {
                        updateBoard(Board, potPins);

                        if (hasBoardChanged(Board, BoardMemory)) {
                            if (isValidPlacement(Board, BoardMemory, currentPlayer, originField)) {
                                displayBoard(lcd, Board);
                                turnOver = true;
                            } else {
                                displayBoard(lcd, BoardMemory);
                                displayIllegalMove(lcd);
                                awaitBoardIsEqual(Board, BoardMemory, potPins);
                                displayPlayer(lcd, gameSettings, currentPlayer);
                            }
                        }
                    } while (!turnOver);
                }
            }
        }

        if (turnOver) {
            turn++;
            int evaluation = evaluateBoard(Board);
            if (evaluation == 10 || evaluation == -10) {
                displayWinner(lcd, gameSettings, evaluation == 10 ? Player1 : Player2);
                break;
            }
            copyBoard(Board, BoardMemory);
            switchPlayer(currentPlayer);
            displayPlayer(lcd, gameSettings, currentPlayer);
            turnOver = false;
        }
    }
}

// Funktion für das Spiel Tic Tac Toe
void playTapatan(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board[3][3], int BoardMemory[3][3], int currentPlayer, const int potPins[]) {
    // Spiellogik für Tic Tac Toe
    if (gameSettings.mode == PlayerVsComputer) {
        // TapatanPlayerVsComputer();                                                                                 // Spieler gegen Computer
    } else if (gameSettings.mode == PlayerVsPlayer) {
        TapatanPlayerVsPlayer(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);                         // Spieler gegen Spieler
    }
}

#endif