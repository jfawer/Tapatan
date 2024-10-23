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

    if (value >= 0 && value <= 350) {                 // Wertebereiche für die Potentiometer
      Board[row][col] = 1;
    } else if (value >= 351 && value <= 700) {  
      Board[row][col] = 0;
    } else if (value >= 701 && value <= 1023) {
      Board[row][col] = 2;
    } else {
      Board[row][col] = -1;                           // Falls der Wert in keinem Bereich liegt
    }
  }
}

#endif
