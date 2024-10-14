#ifndef AUSLESEN_H
#define AUSLESEN_H

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
void copyArray(int Board[3][3], int Boardspeicher[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      Boardspeicher[i][j] = Board[i][j];
    }
  }
}

// Funktion zum Überprüfen, ob sich die Arraywerte geändert haben
bool hasChanged(int Board[3][3], int Boardspeicher[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (Board[i][j] != Boardspeicher[i][j]) {
        return true;
      }
    }
  }
  return false;
}

#endif
