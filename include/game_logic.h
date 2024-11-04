// Funktionen für die Spiellogik

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

// Einbinden der Bibliotheken
#include <Arduino.h>

// Einbinden der Header-Dateien
#include "struct.h"
#include <limits.h>

// ====================================================================================================
// Hilfsfunktionen für die Spiellogik
// ====================================================================================================

// Funktion zum Kopieren eines Arrays in ein anderes Array
void copyBoard(int sourceBoard[3][3], int destinationBoard[3][3]) {  
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      destinationBoard[i][j] = sourceBoard[i][j];
    }
  }
}

// Funktion zum Wechseln des aktuellen Spielers (Von 1 zu 2 und umgekehrt)
void switchPlayer(int &currentPlayer) {
  currentPlayer = (currentPlayer == 1) ? 2 : 1; 
}


// ====================================================================================================
// Funktionen für die Spiellogik des Spielers
// ====================================================================================================

// Funktion zur Erkennund, ob sich das Spielfeld geändert hat
bool hasBoardChanged(int currentBoard[3][3], int savedBoard[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (currentBoard[i][j] != savedBoard[i][j]) {
        return true;
      }
    }
  }
  return false;
}

// Funktion zum Erkennen des geänderten Feldes
BoardField getChangedField(int currentBoard[3][3], int savedBoard[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (currentBoard[i][j] != savedBoard[i][j]) {
                return {i, j}; // Rückgabe des geänderten Feldes
            }
        }
    }
    return {-1, -1}; // Rückgabe von (-1, -1), wenn kein Feld geändert wurde
}

// Funktion zum Überprüfen, ob der Zug des Spielers gültig ist
bool isValidMove(int currentBoard[3][3], int savedBoard[3][3], int currentPlayer) {
    BoardField changedField = getChangedField(currentBoard, savedBoard);
    int row = changedField.row;
    int col = changedField.col;

    if (row == -1 && col == -1) {
        return false; // Kein Feld wurde geändert
    }

    int oldValue = savedBoard[row][col];   
    int newValue = currentBoard[row][col];

    if (oldValue == 0 && newValue == currentPlayer) {   // Gültiger Zug, wenn das Feld leer war und der Spieler das Feld geändert hat
        return true;
    } else {
        return false;
    }
}

// Funktion zum Überprüfen ob das aktuelle Spielfeld dem gespeicherten Spielfeld entspricht
bool isBoardEqual(int currentBoard[3][3], int savedBoard[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (currentBoard[i][j] != savedBoard[i][j]) {
        return false;
      }
    }
  }
  return true;
}

// Funktion zum Warten, bis das Spielfeld wieder dem gespeicherten Spielfeld entspricht
void awaitBoardIsEqual(int currentBoard[3][3], int savedBoard[3][3], const int potPins[]) {
  while (!isBoardEqual(currentBoard, savedBoard)) {
    updateBoard(currentBoard, potPins);
    delay(100);
  }
}

// Funktion zum Überprüfen, ob das Spielfeld geändert wurde und der Zug gültig ist
bool playerMove(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board[3][3], int BoardMemory[3][3], int currentPlayer, const int potPins[]) {
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


// ====================================================================================================
// Funktionen für die Spiellogik des Computers / Spielauswertung
// ====================================================================================================

// Funktion zum Überprüfen, ob noch Felder frei sind
bool checkFieldsLeft(int Board[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (Board[i][j] == 0) {
        return true;
      }
    }
  }
  return false;
}

// Funktion zur Bewertung des Spielfelds
int evaluateBoard(int Board[3][3]) {
    // Überprüfen, ob eine Reihe oder Spalte gewonnen hat
    for (int i = 0; i < 3; i++) {
        if (Board[i][0] == Board[i][1] && Board[i][1] == Board[i][2] && Board[i][0] != 0) {
            return (Board[i][0] == 1) ? 10 : (Board[i][0] == 2) ? -10 : 0;
        }
        if (Board[0][i] == Board[1][i] && Board[1][i] == Board[2][i] && Board[0][i] != 0) {
            return (Board[0][i] == 1) ? 10 : (Board[0][i] == 2) ? -10 : 0;
        }
    }

    // Überprüfen, ob eine Diagonale gewonnen hat
    if (Board[0][0] == Board[1][1] && Board[1][1] == Board[2][2] && Board[0][0] != 0) {
        return (Board[0][0] == 1) ? 10 : (Board[0][0] == 2) ? -10 : 0;
    }
    if (Board[0][2] == Board[1][1] && Board[1][1] == Board[2][0] && Board[0][2] != 0) {
        return (Board[0][2] == 1) ? 10 : (Board[0][2] == 2) ? -10 : 0;
    }
    return 0;
}

// Funktion zur Überprüfung, ob das Spiel vorbei ist
bool isGameOver(int Board[3][3]) {
  // Überprüfen, ob jemand gewonnen hat
  if (evaluateBoard(Board) != 0) {
    return true;
  }
  // Überprüfen, ob noch Felder frei sind
  return !checkFieldsLeft(Board);
}

// Funktion zur Generierung aller möglichen nächsten Zustände
void getChildren(int Board[3][3], int children[9][3][3], BoardField moves[9], int &numChildren, bool maximizingPlayer) {
  numChildren = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (Board[i][j] == 0) {
        // Kopiere das aktuelle Board
        copyBoard(Board, children[numChildren]);
        // Setze den Zug
        children[numChildren][i][j] = maximizingPlayer ? 1 : 2;
        // Speichere den Zug
        moves[numChildren].row = i;
        moves[numChildren].col = j;
        numChildren++;
      }
    }
  }
}

// Minimax-Algorithmus mit Alpha-Beta-Pruning
int minimax(int Board[3][3], int depth, int alpha, int beta, bool maximizingPlayer, BoardField &bestMove) {
  if (depth == 0 || isGameOver(Board)) {
    return evaluateBoard(Board);
  }

  if (maximizingPlayer) {
    int maxEval = INT_MIN;
    BoardField currentMove;
    int children[9][3][3];
    BoardField moves[9];
    int numChildren;
    getChildren(Board, children, moves, numChildren, true);
    for (int i = 0; i < numChildren; i++) {
      int eval = minimax(children[i], depth - 1, alpha, beta, false, currentMove);
      if (eval > maxEval) {
        maxEval = eval;
        bestMove = moves[i]; // Speichere die tatsächliche Position
      }
      alpha = max(alpha, eval);
      if (beta <= alpha) {
        break;
      }
    }
    return maxEval;
  } else {
    int minEval = INT_MAX;
    BoardField currentMove;
    int children[9][3][3];
    BoardField moves[9];
    int numChildren;
    getChildren(Board, children, moves, numChildren, false);
    for (int i = 0; i < numChildren; i++) {
      int eval = minimax(children[i], depth - 1, alpha, beta, true, currentMove);
      if (eval < minEval) {
        minEval = eval;
        bestMove = moves[i]; // Speichere die tatsächliche Position
      }
      beta = min(beta, eval);
      if (beta <= alpha) {
        break;
      }
    }
    return minEval;
  }
}

// Funktion zur Bestimmung eines zufälligen Zuges für den Computer und Einfügen in das Board
void makeRandomMove(int Board[3][3]) {
  BoardField randomMove;
  do {
    randomMove.row = random(0, 3);
    randomMove.col = random(0, 3);
  } while (Board[randomMove.row][randomMove.col] != 0);

  Board[randomMove.row][randomMove.col] = 2;                          // Setze den Zug für Spieler 2
}

// Funktion zur Bestimmung des besten Zuges für den Computer und Einfügen in das Board
void makeBestMove(int Board[3][3]) {
  BoardField bestMove;                                                // Speichert den besten Zug
  minimax(Board, 9, INT_MIN, INT_MAX, false, bestMove);               // Computer (minimizingPlayer) am Zug
  Board[bestMove.row][bestMove.col] = 2;                              // Setze den Zug für den Computer
}


// ====================================================================================================
// Funktionen für das Zurücksetzen der Spieleinstellungen / des Spielfelds
// ====================================================================================================

// Funktion zum Warten auf das Zurücksetzen des Spielfelds
void awaitBoardReset (int Board[3][3], const int potPins[]) {
  int ResetBoard[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
  while (!isBoardEqual(Board, ResetBoard)) {
    updateBoard(Board, potPins);
    delay(100);
  }
}

// Funktion zum resetten der Spieleinstellungen / GameSettings
void resetGameSettings(GameSettings &gameSettings) {
  gameSettings.game = 0;
  gameSettings.mode = 0;
  gameSettings.difficulty = 0;
}


#endif