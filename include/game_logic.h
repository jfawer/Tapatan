// Funktionen für die Spiellogik

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

// Einbinden der Bibliotheken
#include <Arduino.h>

// Einbinden der Header-Dateien
#include "struct.h"
#include <limits.h>

// Funktion zum Kopieren eines Arrays in ein anderes Array
void copyBoard(int Board[3][3], int BoardMemory[3][3]) {  
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      BoardMemory[i][j] = Board[i][j];
    }
  }
}

// Funktion zum Wechseln des aktuellen Spielers (Von 1 zu 2 und umgekehrt)
void switchPlayer(int &currentPlayer) {
  currentPlayer = (currentPlayer == 1) ? 2 : 1; 
}

// Funktion zum Überprüfen, ob sich die Arraywerte / das Spielfeld geändert haben
bool hasChanged(int Board[3][3], int BoardMemory[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (Board[i][j] != BoardMemory[i][j]) {
        return true;
      }
    }
  }
  return false;
}

// Funktion zum Erkennen, welches Feld geändert wurde (Gibt Reihe und Spalte zurück)
BoardField getChangedField(int Board[3][3], int BoardMemory[3][3]) {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
          if (Board[i][j] != BoardMemory[i][j]) {
              return {i, j}; // Rückgabe des geänderten Feldes
          }
      }
    }
  return {-1, -1}; // Rückgabe von (-1, -1), wenn kein Feld geändert wurde
}

// Funktion zum Überprüfen, ob der Zug des Spielers gültig ist
bool isValidMove(int Board[3][3], int BoardMemory[3][3], int currentPlayer) {
    BoardField changedField = getChangedField(Board, BoardMemory);
    int row = changedField.row;
    int col = changedField.col;

    if (row == -1 || col == -1) {
        return false; // Kein gültiger Zug, wenn kein Feld geändert wurde
    }

    int oldValue = BoardMemory[row][col];
    int newValue = Board[row][col];

    if (oldValue == 0 && newValue == currentPlayer) {   // Gültiger Zug, wenn das Feld leer war und der Spieler das Feld geändert hat
        return true;
    } else {
        return false;
    }
}

// Funktion zum Überprüfen ob das aktuelle Spielfeld dem gespeicherten Spielfeld entspricht
bool isBoardEqual(int Board_A[3][3], int Board_B[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (Board_A[i][j] != Board_B[i][j]) {
        return false;
      }
    }
  }
  return true;
}

// Funktion zum Warten, bis der illegale Zug zurückgenommen wurde
void waitUntilMoveIsUndone(int Board[3][3], int BoardMemory[3][3], const int numPotentiometers, const int potPins[]) {
  while (!isBoardEqual(Board, BoardMemory)) {
    readSensors(Board, numPotentiometers, potPins);
    delay(100);
  }
}

// Funktion zum Warten, bis das Spielfeld komplett zurückgesetzt wurde
void waitForReset (int Board[3][3], const int numPotentiometers, const int potPins[]) {
  int ResetBoard[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
  while (!isBoardEqual(Board, ResetBoard)) {
    readSensors(Board, numPotentiometers, potPins);
    delay(100);
  }
}


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

// Funktion zur Bewertung des Spielbretts
int evaluate(int Board[3][3]) {
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
    if (evaluate(Board) != 0) {
        return true;
    }
    // Überprüfen, ob noch Felder frei sind
    return !checkFieldsLeft(Board);
}

// Funktion zur Generierung aller möglichen nächsten Zustände
void getChildren(int Board[3][3], int children[9][3][3], Move moves[9], int &numChildren, bool maximizingPlayer) {
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
int minimax(int Board[3][3], int depth, int alpha, int beta, bool maximizingPlayer, Move &bestMove) {
    if (depth == 0 || isGameOver(Board)) {
        return evaluate(Board);
    }

    if (maximizingPlayer) {
        int maxEval = INT_MIN;
        Move currentMove;
        int children[9][3][3];
        Move moves[9];
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
        Move currentMove;
        int children[9][3][3];
        Move moves[9];
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

// Funktion zur Bestimmung des besten Zuges für den Computer
Move getBestMove(int Board[3][3], int depth) {
    Move bestMove;
    minimax(Board, depth, INT_MIN, INT_MAX, false, bestMove); // Spieler 2 (minimizingPlayer) am Zug
    return bestMove;
}

// Funktion zur Bestimmung des besten Zuges für den Computer und Einfügen in das Board
void makeBestMove(int Board[3][3], int depth) {
    Move bestMove = getBestMove(Board, depth);
    Board[bestMove.row][bestMove.col] = 2; // Setze den Zug für Spieler 2
}


#endif