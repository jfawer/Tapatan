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

/*
void ledStreifenAnsteuern(int r, int g, int b, int redLed, int greenLed, int blueLed) {
  analogWrite(redLed, r);
  analogWrite(greenLed, g);
  analogWrite(blueLed, b);
  
  // Farben:
  //blau: LED(0,0,255);
  //rot: LED(255,0,0);
  //grün: LED(0,255,0);
  //weiss: LED(255,255,255);
  //aus: LED(0,0,0);
}

void LED(int farbe, int ledPins[]) {
  switch (farbe) {
    case 0: ledStreifenAnsteuern(0, 0, 0, ledPins[0], ledPins[1], ledPins[2]); break;
    case 1: ledStreifenAnsteuern(255, 0, 0, ledPins[0], ledPins[1], ledPins[2]); break;
    case 2: ledStreifenAnsteuern(0, 255, 0, ledPins[0], ledPins[1], ledPins[2]); break;
    case 3: ledStreifenAnsteuern(0, 0, 255, ledPins[0], ledPins[1], ledPins[2]); break;
    case 4: ledStreifenAnsteuern(255, 255, 255, ledPins[0], ledPins[1], ledPins[2]); break;
  }
}
*/

#endif