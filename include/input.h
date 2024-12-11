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

    if (value >= 0 && value <= 400) {                                                           // Überprüfen, in welchem Bereich der Wert liegt
      Board[row][col] = 1;                                                                      // Wert 1 für Spieler 1
    } else if (value >= 401 && value <= 599) {    
      Board[row][col] = 0;                                                                      // Wert 0 für leeres Feld
    } else if (value >= 600 && value <= 1023) {
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
  int minVal = 0;                                                                               // Minimaler Wert des Rotary-Encoders
  int maxVal = 24;                                                                              // Maximaler Wert des Rotary-Encoders

  if (digitalRead(rotarySwitchPin1) == LOW) {                                                   // Überprüfen, ob der Pin auf LOW ist
    if (digitalRead(rotarySwitchPin2) == LOW) {                                                 // Richtung des Encoders auslesen
      // Rechtsdrehung
      rotarySwitchValue++;                                                                      // Wert erhöhen
      if (rotarySwitchValue > maxVal) {
          rotarySwitchValue = maxVal;
      }
    } else {
      // Linksdrehung
      rotarySwitchValue--;                                                                      // Wert verringern
      if (rotarySwitchValue < minVal) {
          rotarySwitchValue = minVal;
      }
    }
  }
}

// Funktion zur Berechnung des Bereichs des Rotary-Encoders
int getRotarySwitchRangeValue(int numRange, int rotarySwitchValue) {
  int maxVal = 24;                                                                              // Maximale Wert des Rotary-Encoders
  int rangeSize = maxVal / numRange;                                                            // Grösse der Bereiche

  for (int i = 0; i < numRange; i++) {                                                          // Durchlaufe alle Bereiche
    int rangeStart = i * rangeSize;                                                             // Start des aktuellen Bereichs
    int rangeEnd = (i + 1) * rangeSize;                                                         // Ende des aktuellen Bereichs (inklusiv)

    if (i == numRange - 1) {                                                                    // Stelle sicher, dass der letzte Bereich maxVal enthält
      rangeEnd = maxVal;
    }

    if (rotarySwitchValue >= rangeStart && rotarySwitchValue <= rangeEnd) {                     // Prüfe, ob der Wert im Bereich liegt
      return i + 1;                                                                             // Gib den Bereichsindex (beginnend bei 1) zurück
    }
  }
  return -1;                                                                                    // Rückgabewert bei ungültigem rotarySwitchValue
}

#endif