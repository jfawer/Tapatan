// Funktionen zum Auslesen der Sensorwerte

#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>

// Funktion zum Auslesen der Sensorwerte und aktualisieren des Board-Arrays
void updateBoard(int Board[3][3], const int potPins[]) {
  const int anzahlPotentiometer = 9;                                                            // Anzahl der Potentiometer
  for (int i = 0; i < anzahlPotentiometer; i++) {                                               // Potentiometer durchgehen
    int value = analogRead(potPins[i]);                                                         // Wert des Potentiometers auslesen
    int row = i / 3;                                                                            
    int col = i % 3;

    if (value >= 0 && value <= 350) {                                                           // Überprüfen, in welchem Bereich der Wert liegt
      Board[row][col] = 1;                                                                      // Wert 1 für Spieler 1
    } else if (value >= 351 && value <= 700) {    
      Board[row][col] = 0;                                                                      // Wert 0 für leeres Feld
    } else if (value >= 701 && value <= 1023) {
      Board[row][col] = 2;                                                                      // Wert 2 für Spieler 2
    } else {
      Board[row][col] = -1;                                                                     // Wert -1 für Fehler
    }
  }
}

// Funktion zum Erkennen einer fallenden Flanke
bool fallendeFlanke(int buttonPin) {
  static bool lastButtonState = HIGH;                                                           // Speichert den letzten Zustand des Buttons
  bool buttonState = digitalRead(buttonPin);                                                    // Liest den aktuellen Zustand des Buttons aus
  if (lastButtonState == HIGH && buttonState == LOW) {                                          // Überprüft, ob eine fallende Flanke erkannt wurde
    lastButtonState = buttonState;
    return true;                                                                                // Gibt true zurück, wenn eine fallende Flanke erkannt wurde
  } else {
    lastButtonState = buttonState;
    return false;                                                                               // Gibt false zurück, wenn keine fallende Flanke erkannt wurde
  }
}

// Funktion zum Auslesen des Rotary-Encoders
void getRotaryValue(int rotarySwitchPin1, int rotarySwitchPin2, volatile int &rotarySwitchValue) {
  if (digitalRead(rotarySwitchPin1) == LOW) {                                                   // Überprüfen, ob der Pin auf LOW ist
    if (digitalRead(rotarySwitchPin2) == LOW) {                                                 // Richtung des Encoders auslesen
      // Rechtsdrehung
      rotarySwitchValue++;                                                                      // Wert erhöhen
      if (rotarySwitchValue > 40) {
          rotarySwitchValue = 0;
      }
    } else {
      // Linksdrehung
      rotarySwitchValue--;                                                                      // Wert verringern
      if (rotarySwitchValue < 0) {
          rotarySwitchValue = 40;
      }
    }
  }
}

// Funktion zur Berechnung des Bereichs des Rotary-Encoders
int getRotarySwitchRangeValue(int numRange, int rotarySwitchValue) {

  int rangeSize = 40 / numRange;                                                                // Grösse der Bereiche
  int midRangeSize = rangeSize/2;                                                               // Grösse des mittleren Bereichs

  if(rotarySwitchValue <= midRangeSize || rotarySwitchValue >= (40-midRangeSize)){              // Überprüfen, ob der Wert im ersten oder letzten Bereich liegt
    return 1;
  }

  for (int i = 0; i < numRange-1; i++) {                                                        // Überprüfen, in welchem Bereich der Wert liegt
    if(rotarySwitchValue >= (i * rangeSize) + midRangeSize && rotarySwitchValue <= ((i + 1) * rangeSize) + midRangeSize){
      return i + 2;
    }
  }
  return 0;
}

#endif