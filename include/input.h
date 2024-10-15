// Funktionen zum Auslesen der Sensorwerte

#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>

// Funktion zum Auslesen der Sensorwerte
void readSensors(int Board[3][3], const int numPotentiometers, const int potPins[]) {
  for (int i = 0; i < numPotentiometers; i++) {
    int value = analogRead(potPins[i]);
    int row = i / 3;
    int col = i % 3;
    if (value >= 0 && value <= 350) {
      Board[row][col] = 1;
    } else if (value >= 351 && value <= 700) {
      Board[row][col] = 0;
    } else if (value >= 701 && value <= 1023) {
      Board[row][col] = 2;
    } else {
      Board[row][col] = -1; // Falls der Wert in keinem Bereich liegt
    }
  }
}

// Funktion zum Übertragen der Arraywerte eines Arrays in ein anderes Array
void copyArray(int Board[3][3], int BoardSpeicher[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      BoardSpeicher[i][j] = Board[i][j];
    }
  }
}

// Funktion zum Überprüfen, ob sich die Arraywerte geändert haben
bool hasChanged(int Board[3][3], int BoardSpeicher[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (Board[i][j] != BoardSpeicher[i][j]) {
        return true;
      }
    }
  }
  return false;
}

// Funktion zum Erkennen, welches Feld geändert wurde
void getChangedField(int Board[3][3], int BoardSpeicher[3][3], int &row, int &col) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (Board[i][j] != BoardSpeicher[i][j]) {
        row = i;
        col = j;
        return;
      }
    }
  }
}

// Funktion zum Überprüfen, ob der Zug gültig ist
bool isValidMove(int Board[3][3], int BoardSpeicher[3][3], int row, int col, int turn) {
  int SpielerNummer = turn;
  int oldValue = BoardSpeicher[row][col];
  int newValue = Board[row][col];

  if (oldValue == 0 && newValue == SpielerNummer) {
    return true;
  } else {
    return false;
  }
}

// Funktion zum Überprüfen ob das akteulle Spielfeld mit dem Spielfeld aus dem Speicher übereinstimmt
bool isBoardEqual(int Board[3][3], int BoardSpeicher[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (Board[i][j] != BoardSpeicher[i][j]) {
        return false;
      }
    }
  }
  return true;
}

#endif
