// Funktionen zum Auslesen der Sensorwerte

#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include "struct.h"

void steuereLED(int farbe, int ledPins[]) {
  int r = 0, g = 0, b = 0;                        // Standardfarbe: aus (0, 0, 0)

  switch (farbe) {
    case Aus: break;                              // Aus
    case Rot: r = 255; break;                     // Rot
    case Gruen: g = 255; break;                   // Grün
    case Blau: b = 255; break;                    // Blau
    case Weiss: r = g = b = 255; break;           // Weiss
    default: break;                               // Aus
  }

  analogWrite(ledPins[0], r);                     // Roten Kanal steuern
  analogWrite(ledPins[1], g);                     // Grünen Kanal steuern
  analogWrite(ledPins[2], b);                     // Blauen Kanal steuern
}

#endif