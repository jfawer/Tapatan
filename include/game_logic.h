// Funktionen für die Spiellogik

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

// Einbinden der Bibliotheken
#include <Arduino.h>

// Einbinden der Header-Dateien
#include "struct.h"

// Funktion zum Wechseln des aktuellen Spielers (Von 1 zu 2 und umgekehrt)
void switchPlayer(int &currentPlayer) {
  currentPlayer = (currentPlayer == 1) ? 2 : 1; 
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
        if (Board[i][0] == Board[i][1] && Board[i][1] == Board[i][2]) {
            return (Board[i][0] == 1) ? 10 : (Board[i][0] == 2) ? -10 : 0;
        }
        if (Board[0][i] == Board[1][i] && Board[1][i] == Board[2][i]) {
            return (Board[0][i] == 1) ? 10 : (Board[0][i] == 2) ? -10 : 0;
        }
    }

    // Überprüfen, ob eine Diagonale gewonnen hat
    if (Board[0][0] == Board[1][1] && Board[1][1] == Board[2][2]) {
        return (Board[0][0] == 1) ? 10 : (Board[0][0] == 2) ? -10 : 0;
    }
    if (Board[0][2] == Board[1][1] && Board[1][1] == Board[2][0]) {
        return (Board[0][2] == 1) ? 10 : (Board[0][2] == 2) ? -10 : 0;
    }

    return 0;
}

// Funktion zur Ausführung des Minimax Algorithmus
int minmax(int Board[3][3], int depth, bool isMax) {                            
  int score = evaluate(Board);

  if (score == 10 || score == -10) {                                            // Überprüfen, ob das Spiel beendet ist
    return score;
  }

  if (!checkFieldsLeft(Board)) {                                                // Überprüfen, ob es noch Züge gibt
    return 0;                                                                   // Unentschieden
  }

  if (isMax) {                                                                  // Maximierender Spieler
    int best = -1000;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (Board[i][j] == 0) {
          Board[i][j] = 1;                                                      // Zug ausführen
          best = max(best, minmax(Board, depth + 1, !isMax));                   // Rekursiver Aufruf
          Board[i][j] = 0;                                                      // Zug zurücknehmen
        }
      }
    }
    return best;
  } else {                                                                      // Minimierender Spieler
    int best = 1000;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (Board[i][j] == 0) {
          Board[i][j] = 2;                                                      // Zug ausführen
          best = min(best, minmax(Board, depth + 1, !isMax));                   // Rekursiver Aufruf
          Board[i][j] = 0;                                                      // Zug zurücknehmen
        }
      }
    }
    return best;
  }
}

// Funktion für den Computerzug
void ComputerTurn(int Board[3][3], bool &firstMove) {
    int bestRow = 0;
    int bestCol = 0;

    if (firstMove) {
        bool played = false;  // Überprüfen, ob die KI eine unbesetzte Ecke gefunden hat
        int temp = random(4);  // Zufällige Auswahl einer der vier Ecken
        if (temp == 0) {
            if (Board[0][0] == 0) {  // Überprüfen, ob die Ecke frei ist
                bestRow = 0;
                bestCol = 0;
                played = true;
            }
        } else if (temp == 1) {
            if (Board[0][2] == 0) {  // Überprüfen, ob die Ecke frei ist
                bestRow = 0;
                bestCol = 2;
                played = true;
            }
        } else if (temp == 2) {
            if (Board[2][0] == 0) {  // Überprüfen, ob die Ecke frei ist
                bestRow = 2;
                bestCol = 0;
                played = true;
            }
        } else {
            if (Board[2][2] == 0) {  // Überprüfen, ob die Ecke frei ist
                bestRow = 2;
                bestCol = 2;
                played = true;
            }
        }
        if (!played) {  // Wenn keine Ecke frei ist, wähle das mittlere Feld
            bestRow = 1;
            bestCol = 1;
        }
        firstMove = false;  // Setze firstMove auf false, da der erste Zug abgeschlossen ist
    } else {
        int bestVal = 1000;  // Initialisiere bestVal mit einem hohen Wert
        for (int i = 0; i < 3; i++) {  // Durchlaufe jede Zeile
            for (int j = 0; j < 3; j++) {  // Durchlaufe jede Spalte
                if (Board[i][j] == 0) {  // Wenn das Feld frei ist
                    Board[i][j] = 2;  // Führe den Zug aus
                    int moveVal = minmax(Board, 0, true);  // Berechne den Wert des Zuges
                    Board[i][j] = 0;  // Nimm den Zug zurück

                    // Aktualisiere die besten Koordinaten und den besten Wert
                    if (moveVal < bestVal) {
                        bestRow = i;
                        bestCol = j;
                        bestVal = moveVal;
                    }
                }
            }
        }
    }
    Board[bestRow][bestCol] = 2;  // Führe den besten Zug aus
}

#endif